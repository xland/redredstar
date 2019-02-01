<template>
    <div :class="initWebview?'tarSiteMask_ing':'tarSiteMask'">
        <div class="tarSiteMaskClose" @click="$parent.showSites = false">
            <i class="iconfont icon-guanbi" style="font-size: 17px !important;"></i>
        </div>
        <div class="z1">
            <div class="tarSiteMaskHeader" v-html="publishText"></div>
            <div v-show="!initWebview" class="tarSiteContainer">
                <div @click="publish(item)" :class="['tarSiteItem',item.component?'':'notReady']" v-for="(item,index) in sites">
                    <div class="tarSiteIcon">
                        <img :src="'static/icons/'+item.id+'.png'" />
                    </div>
                    <div class="tarSiteName">
                        <i :class="['iconfont',item.component?'icon-eye green':'icon-eyeslash gray']"></i>
                        {{item.title}}
                    </div>
                </div>
            </div>
        </div>
        <component class="webview" v-if="initWebview" :is="component"></component>
    </div>
</template>
<script>
    import cnblogs from "./sites/cnblogs";
    import jianshu from "./sites/jianshu";
    import oschina from "./sites/oschina";
    import swal from 'sweetalert';
    export default {
        components: {
            cnblogs,
            jianshu,
            oschina
        },
        data() {
            return {
                initWebview: false,
                component: null,
                publishText: '文章发布至...',
                sites: [{
                    id: 'jianshu',
                    title: '简书',
                    component: jianshu,
                }, {
                    id: 'cnblogs',
                    title: '博客园',
                    component: cnblogs,
                }, {
                    id: 'oschina',
                    title: '开源中国',
                    component:oschina,
                }, {
                    id: 'weixin',
                    title: '微信公众号',
                }, {
                    id: 'zhihu',
                    title: '知乎',
                }, {
                    id: 'csdn',
                    title: 'CSDN',
                }, {
                    id: 'segmentfault',
                    title: '思否',
                }, {
                    id: 'douban',
                    title: '豆瓣',
                }, {
                    id: 'wordpress',
                    title: 'wordpress',
                }, {
                    id: 'qqzone',
                    title: 'QQ空间',
                }]
            }
        },
        mounted() {},
        methods: {
            publish(item) {
                if (!item.component) {
                    swal({
                        icon: "error",
                        text: "暂时还不支持发布至" + item.title,
                    });
                    return;
                }
                this.publishText = "知识发布至" + item.title;
                this.component = item.component;
                this.initWebview = true;
            },
            prepareImgSrc(site) {
                var content = UE.instants.ueditorInstant0.getContent();
                content = content.replace(/src="file:.+?"/gi,'');
                var re = new RegExp("data-img_"+site,"gi");
                content = content.replace(re,'src');
                content = content.replace(/data-img_.+?=".+?"/gi,'');
                return content;
            }
        }
    }
</script>
<style lang="scss" scoped>
    .tarSiteMask_ing {
        position: absolute;
        top: 24px;
        left: 0px;
        right: 0px;
        bottom: 26px;
        background: #ccc;
        z-index: 99;
        display: flex;
        flex-direction: column;
    }
.green{
    color: green;
}
.gray{
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
        margin-left: 38px;
        margin-right: 38px;
        display: flex;
        flex-wrap:wrap;
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
    .tarSiteMaskHeader {
        width: 100%;
        margin-top: 8px;
        height: 80px;
        text-align: center;
        line-height: 80px;
        font-size: 38px;
        color: #fff;
        font-weight: bolder;
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
    .notReady:hover{
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

    .webview {
        position: absolute;
        top: 0px;
        left: 0px;
        bottom: 0px;
        right: 0px;
        z-index: 1;
    }
</style>