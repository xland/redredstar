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
  init(setting) {
    this.tickStep = setting.autosave_interval * 1000;
    this.imgHight = setting.img_h;
    this.imgWidth = setting.img_w;
    this.editorType = setting.editor_type;
    this.jnaSync = setting.jna_sync;
    this.jnaToken = setting.jna_token;
    if (!setting.jna_login_show) {
      swal({
        icon: "info",
        text: "为了兼容未来更多酷炫的功能\n我们希望您先登陆",
        closeOnClickOutside: false,
        closeOnEsc: false,
        buttons: [
          "就想用单机版", "这就去扫码登陆"
        ]
      }).then((value) => {
        this.db("settings").update({
          "jna_login_show": true
        }).then();
        if (!value) return;
        this.bus.$emit('login');
      });
    }
  },
  async delNoReferTag(tagId) {
    let count = await this.db("flower_tag").count('id as count').where("tag_id", tagId);
    count = count[0].count;
    if (count > 0) return;
    count = await this.db("article_tag").count('id as count').where("tag_id", tagId);
    count = count[0].count;
    if (count > 0) return;
    this.db("tags").where("id", tagId).del().then();
    let index = this.tags.findIndex(v => v.id = tagId);
    this.tags.splice(index, 1);
    this.bus.$emit('removeTag', tagId);
  },
  articleFromWebApp(e, message) {
    let article = {
      title: message.title,
      created_at: new Date(),
      updated_at: new Date(),
      editor_type: "html",
    };
    this.db("articles").insert(article).then(rows => {
      article.id = rows[0];
      let aPath = path.join(remote.app.getPath('userData'), "/xxm/" + article.id);
      fs.mkdirSync(aPath);
      fs.writeFileSync(path.join(aPath, "/a.data"), message.content, this.$root.rwOption);
      this.bus.$emit('articleCount');
      this.bus.$emit('newArticleAdd');
    })
  },
  updateVersion(e, message) {
    swal({
      icon: "info",
      text: "新版本已经为您准备好啦！\n现在升级？还是下次启动应用时再升级？",
      closeOnClickOutside: false,
      closeOnEsc: false,
      buttons: [
        "下次升级", "现在升级"
      ]
    }).then((value) => {
      if (!value) return;
      ipcRenderer.send('updateMain');
    });
  },
  hookMsgFromMainProcess() {
    ipcRenderer.on('articlePublishMsgFromMain', (e, message) => this.articlePublushCb(message))
    ipcRenderer.on('imgUploadMsgFromMain', (e, message) => this.imgUploadCb(message));
    ipcRenderer.on("alertMsgFromMain", (e, message) => swal(message));
    ipcRenderer.on('articleFromWebApp', this.articleFromWebApp);
    ipcRenderer.on('updateMsgFromMain', this.updateVersion);
  }
}
export default store