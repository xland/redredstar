<template>
    <div :class="initWebview?'tarSiteMask_ing':'tarSiteMask'">
        <div class="tarSiteMaskClose" @click="$parent.showSites = false">
            <i class="iconfont icon-guanbi" style="font-size: 17px !important;"></i>
        </div>
        <div @click='$parent.showSites = false' class="z1">
            <div v-show="!initWebview" class="tarSiteContainer">
                <div @click.stop @mouseover="overIndex = index" @mouseout="overIndex = -1" :class="['tarSiteItem',item.ready?'':'notReady']" v-for="(item,index) in sites">
                    <div class="tarSiteIcon">
                        <img :src="'./static/sites/'+item.id+'/logo.png'" />
                    </div>
                    <div class="tarSiteName">
                        <div v-show="overIndex != index || !item.ready" class="toolText">
                            {{item.title}}
                        </div>
                        <div @click.stop="publish(item)" v-show="showEditBtn(index) && item.ready" class="toolBtn">修改</div>
                        <div @click.stop="publish(item)" v-show="overIndex == index && item.ready" class="toolBtn">新增</div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>
<script>
    var fs = require('fs');
    var path = require('path');
    import swal from 'sweetalert';
    import sites from '../utils/site';
    const {
        BrowserWindow
    } = require('electron').remote
    export default {
        data() {
            return {
                overIndex:-1,
                winOption: {
                    width: 1056,
                    height: 680,
                    webPreferences: {
                        nodeIntegration: false,
                        preload: null,
                    }
                },
                urlOption: {
                    userAgent: 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36'
                },
                initWebview: false,
                sites: sites,
                article,
            }
        },
        methods: {
            showEditBtn(index){
                if(index == this.overIndex){
                    return true;
                }
            },
            makeWin(item, url, type) {
                this.winOption.webPreferences.preload = path.join(__static, 'sites/' + item.id + '/inject.js');
                let win = new BrowserWindow(this.winOption);
                item.winId = win.id;
                win.on('closed', () => {
                    item.winId = null;
                    win = null
                })
                win.loadURL(url, this.urlOption);
                let content = "";
                if (this.$parent.article.editor_type == "html") {
                    content = window.editorU.getContent();
                } else {
                    content = window.editorMd.getHtml();
                }
                let self = this;
                win.webContents.on('dom-ready', () => {
                    win.webContents.send('message', {
                        title: self.$parent.article.title,
                        id: self.$route.params.id,
                        winId: item.winId,
                        siteId: item.id,
                        url,
                        type,
                        content
                    });
                });
            },
            publish(item) {
                if (!item.ready) {
                    swal({
                        icon: "error",
                        text: "暂时还不支持发布至" + item.title,
                    });
                    return;
                }
                if (item.winId) {
                    var win = BrowserWindow.fromId(item.winId);
                    win.focus();
                    return;
                }
                let url = item.url;
                let type = "new"
                let articleId = this.$route.params.id;
                this.db("article_site").where("article_id", articleId)
                    .andWhere("site_id", item.id).then(rows => {
                        if (rows.length > 0) {
                            url = rows[0].edit_url;
                            type = "edit";
                        }
                        this.makeWin(item, url, type);
                    });
            }
        }
    }
</script>
<style lang="scss" scoped>
    .tarSiteMask_ing {
        position: absolute;
        top: 34px;
        left: 0px;
        right: 0px;
        bottom: 26px;
        background: #ccc;
        z-index: 99;
        display: flex;
        flex-direction: column;
    }

    .green {
        color: green;
    }

    .gray {
        color: #ccc;
    }

    .tarSiteMask {
        position: absolute;
        top: 41px;
        left: 8px;
        right: 8px;
        border-radius: 3px;
        bottom: 34px;
        background: #ccc;
        z-index: 99;
        display: flex;
        flex-direction: column;
    }

    .tarSiteContainer {
        text-align: center;
        margin-top: 60px;
        margin-left: 38px;
        margin-right: 38px;
        display: flex;
        flex-wrap: wrap;
        align-content: center;
        justify-content: center;
    }

    .tarSiteItem {
        width: 96px;
        min-width: 96px;
        height: 88px;
        background: #fff;
        overflow: hidden;
        border-radius: 3px;
        margin: 12px;
        cursor: pointer;
    }

    .tarSiteMaskClose {
        z-index: 999;
        border-radius: 13px;
        cursor: pointer;
        position: absolute;
        color: #ee4242;
        right: 0px;
        width: 26px;
        height: 26px;
        text-align: center;
        line-height: 26px;
    }

    .z1 {
        position: absolute;
        background: #ccc;
        top: 0px;
        left: 0px;
        bottom: 0px;
        right: 0px;
        z-index: 9;
        border-radius: 3px;
    }

    .tarSiteMaskClose:hover {
        color: red;
    }

    .tarSiteItem:hover {
        box-shadow: 0 3px 6px rgba(16, 16, 16, 0.6);
    }

    .tarSiteIcon {
        height: 56px;
        line-height: 56px;
        text-align: center;
    }

    .notReady {
        filter: blur(2px);
        cursor: default;
    }

    .notReady:hover {
        box-shadow: none;
    }

    .tarSiteIcon img {
        margin-top: 6px;
        height: 30px;
        width: 30px;
        vertical-align: middle;
    }

    .tarSiteName {
        height: 32px;
        line-height: 32px;
        text-align: center;
        color: #555;
        display: flex;
    }
    .toolText{
        flex: 1;
        text-align: center;
    }
    .toolBtn{
        flex: 1;
    }
    .toolBtn:hover{
        border-radius: 3px;
        background: #e7f3ff;
        color: #007acc;
    }
</style>