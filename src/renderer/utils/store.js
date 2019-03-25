const store = {
    tickStep: 8000,
    imgHight: 800,
    imgWidth: 1300,
    editorType: 'html',
    syncJna:true,
    rwOption: {
        encoding: 'utf8'
    },
    curArticleMd:false,
    init(setting){
        this.tickStep = setting.autosave_interval * 1000;
        this.imgHight = setting.img_h;
        this.imgWidth = setting.img_w;
        this.editorType = setting.editor_type;
        this.jnaSync = setting.jna_sync;
        this.jnaToken = setting.jna_token;
    }
}
export default store