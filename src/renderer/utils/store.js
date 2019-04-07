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
  }
}
export default store