<template>
    <div :class="initWebview?'tarSiteMask_ing':'tarSiteMask'">
        <div class="tarSiteMaskClose" @click="$parent.showSites = false">
            <i class="iconfont icon-guanbi" style="font-size: 17px !important;"></i>
        </div>
        <div @click='$parent.showSites = false' class="z1">
            <div v-show="!initWebview" class="tarSiteContainer">
                <div @click.stop="publish(item)" :class="['tarSiteItem',item.ready?'':'notReady']" v-for="(item,index) in sites">
                    <div class="tarSiteIcon">
                        <img :src="'./static/sites/'+item.id+'/logo.png'" />
                    </div>
                    <div class="tarSiteName">
                        {{item.title}}
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
                initWebview: false,
                sites: sites,
            }
        },
        mounted() {},
        methods: {
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
                self = this;
                var win = new BrowserWindow({
                    width: 1056,
                    height: 680,
                    webPreferences: {
                        nodeIntegration: false,
                        preload: path.join(__static, 'sites/' + item.id + '/inject.js')
                    }
                });
                item.winId = win.id;
                win.on('closed', () => {
                    item.winId = null;
                    win = null
                })
                var url = item.url;
                var type = "new"
                if(self.$root.a[self.$root.aIndex][item.id]){
                    url = self.$root.a[self.$root.aIndex][item.id].url;
                    type = "edit";
                }
                win.loadURL(url, {
                    userAgent: 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36'
                });
                win.webContents.on('dom-ready', () => {
                    win.webContents.send('message', {
                        title: self.$root.a[self.$root.aIndex].title,
                        content: window.UE.instants.ueditorInstant0.getContent(),
                        id: self.$root.a[self.$root.aIndex].id,
                        winId: item.winId,
                        siteId: item.id,
                        url,
                        type
                    });
                });
            },
            prepareImgSrc(site) {
                var content = UE.instants.ueditorInstant0.getContent();
                content = content.replace(/src="file:.+?"/gi, '');
                var re = new RegExp("data-img_" + site, "gi");
                content = content.replace(re, 'src');
                content = content.replace(/data-img_.+?=".+?"/gi, '');
                return content;
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
        margin-top: 80px;
        margin-left: 38px;
        margin-right: 38px;
        display: flex;
        flex-wrap: wrap;
        align-content: center;
        justify-content: center;
    }

    .tarSiteItem {
        width: 106px;
        min-width: 106px;
        height: 98px;
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
        height: 32px;
        width: 32px;
        margin-top: 22px;
    }

    .tarSiteName {
        height: 36px;
        line-height: 36px;
        text-align: center;
        color: #666;
    }
</style>