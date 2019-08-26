const {
    ipcRenderer,
    remote
} = require('electron');
const path = require('path');
const fs = require('fs-extra');
const store = {
    tickStep: 8000,
    imgHight: 800,
    imgWidth: 1300,
    editorType: 'html',
    jnaSync: true,
    jnaToken: null,
    tags: [],
    rwOption: {
        encoding: 'utf8'
    },
    curArticleMd: false,
    imgUploadCb: null,
    articlePublushCb: null,
    async init(setting) {
        this.tickStep = setting.autosave_interval * 1000;
        this.imgHight = setting.img_h;
        this.imgWidth = setting.img_w;
        this.editorType = setting.editor_type;
        this.jnaSync = setting.jna_sync;
        this.jnaToken = setting.jna_token;
        if (!setting.jna_login_show) {
            let value = await swal({
                icon: "info",
                text: "为了兼容未来更多酷炫的功能\n我们希望您先登陆",
                closeOnClickOutside: false,
                closeOnEsc: false,
                buttons: [
                    "就想用单机版", "这就去扫码登陆"
                ]
            });
            await this.$root.db("settings").update({ "jna_login_show": true })
            if (!value) return;
            this.bus.$emit('login');
        }
    },
    async delNoReferTag(tagId) {
        let count = await this.$root.db("flower_tag").count('id as count').where("tag_id", tagId);
        count = count[0].count;
        if (count > 0) return;
        count = await this.$root.db("article_tag").count('id as count').where("tag_id", tagId);
        count = count[0].count;
        if (count > 0) return;
        await this.$root.db("tags").where("id", tagId).del();
        let index = this.tags.findIndex(v => v.id = tagId);
        this.tags.splice(index, 1);
        this.bus.$emit('removeTag', tagId);
        this.bus.$emit("tagCount");
    },
    async articleFromWebApp(e, message) {
        let article = {
            title: message.title,
            created_at: new Date(),
            updated_at: new Date(),
            from_url: message.from_url,
            editor_type: "html",
        };
        let [id] = await this.$root.db("articles").insert(article);
        article.id = id;
        let aPath = path.join(remote.app.getPath('userData'), "/xxm/" + article.id);
        fs.mkdirSync(aPath);
        fs.writeFileSync(path.join(aPath, "/a.data"), message.content, this.$root.rwOption);
        this.bus.$emit('articleCount');
        this.bus.$emit('newItemAdd');
    },
    async flowerFromWebApp(e, message) {
        let now = new Date();
        let flower = {
            content: message.content,
            from_url: message.from_url,
            updated_at: now,
            created_at: now
        };
        await this.$root.db("flowers").insert(flower);
        this.bus.$emit('flowerCount');
        this.bus.$emit('newItemAdd');
    },
    async updateVersion(e, message) {
        let value = await swal({
            icon: "info",
            text: "新版本已经为您准备好啦！\n现在升级？还是下次启动应用时再升级？",
            closeOnClickOutside: false,
            closeOnEsc: false,
            buttons: [
                "下次升级", "现在升级"
            ]
        });
        if (!value) return;
        ipcRenderer.send('updateMain');
    },
    hookMsgFromMainProcess() {
        //文章发布插件发布文章到目标平台后，触发articlePublishMsgFromMain，此时画面必然处于文章编辑状态
        ipcRenderer.on('articlePublishMsgFromMain', (e, message) => this.articlePublushCb(message));
        //文章发布插件上传文章内图片到目标平台后，触发imgUploadMsgFromMain，此时画面必然处于文章编辑状态
        ipcRenderer.on('imgUploadMsgFromMain', (e, message) => this.imgUploadCb(message));
        ipcRenderer.on("alertMsgFromMain", (e, message) => swal(message));
        ipcRenderer.on('articleFromWebApp', (e, message) => this.articleFromWebApp(e, message));
        ipcRenderer.on('flowerFromWebApp', (e, message) => this.flowerFromWebApp(e, message));
        ipcRenderer.on('updateMsgFromMain', (e, message) => this.updateVersion(e, message));
    }
}
export default store