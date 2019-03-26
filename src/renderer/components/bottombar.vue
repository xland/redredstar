<template>
    <div class="bottombar">
        <div class="bottombarRight">
            <div @click="setting">
                <i class="iconfont icon-shezhi"></i> Setting
            </div>
            <div @click="addBug">
                <i class="iconfont icon-bug icon"></i> Bug
            </div>
            <div @click="showQrCode('donateQrCode')">
                <i class="iconfont icon-juanzeng icon"></i> Donate
            </div>
            <div>
                <i class="iconfont icon-version icon"></i> {{getVersion()}}
            </div>
        </div>
        <div class="bottombarLeft">
            <div>
                <i class="iconfont icon-wenzhang icon"></i>
                {{articleCount}}
            </div>
            <div>
                <i class="iconfont icon-biaoqian icon"></i>
                {{tagCount}}
            </div>
            <!-- <div>
                <i class="iconfont icon-zishu icon"></i>
            </div> -->
            <div :class="rotating?'rotating':''">
                <i class='iconfont icon-baocun icon'></i>
            </div>
        </div>
        <div style="display: none">
            <div id="donateQrCode">
                <div>
                    <img @click="donateIndex = donateIndex==0?1:0" :src="donates[donateIndex].pic" />
                </div>
                <div>
                    {{donates[donateIndex].name}}赞助
                </div>
                <div class="tip">
                    点击二维码切换为{{donates[donateIndex==0?1:0].name}}赞助
                </div>
            </div>
            <div id="loginQrCode">
                <iframe src="https://jiaonia.com/Xxm/Login" />
            </div>
        </div>
    </div>
</template>
<script>
    import swal from 'sweetalert';
    var electron = require('electron');
    export default {
        data() {
            return {
                rotating: false,
                articleCount: 0,
                tagCount: 0,
                donates: [{
                    pic: './static/imgs/zhifubao.png',
                    name: "支付宝"
                }, {
                    pic: './static/imgs/weixin.png',
                    name: "微信"
                }],
                donateIndex: 1
            }
        },
        methods: {
            hookEvent() {
                this.bus.$on('articleCount', () => {
                    this.db("articles").count('id as count').then(rows => {
                        this.articleCount = rows[0].count;
                    });
                });
                this.bus.$on('tagCount', () => {
                    this.db("tags").count('id as count').then(rows => {
                        this.tagCount = rows[0].count;
                    })
                });
                this.bus.$on('saveContent', () => {
                    this.rotating = true;
                    setTimeout(() => {
                        this.rotating = false
                    }, 600)
                });
                this.bus.$on('login', () => this.showQrCode('loginQrCode'));
                this.bindOnLogin();
            },
            bindOnLogin(){
                let self = this;
                window.addEventListener('message', function (e) {
                    if (!e.data.refuse) {
                        return;
                    }
                    if (e.data.refuse == "True") {
                        return;
                    } else {
                        self.$root.jnaToken = e.data.sid;
                        self.db("settings")
                            .update({
                                "jna_token": e.data.sid
                            }).then();
                        swal.close()
                    }
                }, false);
            },
            setting() {
                this.bus.$emit('findOrAddTab', {
                    url: '/setting',
                    title: "系统设置",
                });
            },
            showQrCode(id) {
                swal({
                    width: 580,
                    content: document.getElementById(id),
                    buttons: false
                });
            },
            addBug() {
                electron.remote.shell.openExternal("https://github.com/xland/xiangxuema/issues");
            },
            getVersion() {
                return electron.remote.app.getVersion();
            },
        },
        mounted() {
            this.hookEvent();
            this.bus.$emit('articleCount');
            this.bus.$emit('tagCount');
        }
    }
</script>
<style scoped lang="scss">
    @keyframes rotating {
        from {
            transform: rotate(0deg);
        }

        to {
            transform: rotate(360deg);
        }
    }

    .rotating {
        animation: rotating 0.6s linear infinite;
        animation-iteration-count: 1;
        animation-fill-mode: forwards;
        display: inline-block;
    }

    .bottombar {
        border-top: 1px solid #e6e6e6;
        background: #f6f6f6;
        color: #999;
        height: 26px;
        line-height: 26px;
        font-size: 12px;
    }

    .bottombarRight {
        float: right;
        width: 280px;
        text-align: right;
        padding-right: 8px;
    }

    .bottombarRight div,
    .bottombarLeft div {
        display: inline-block;
        margin-left: 6px;
        margin-right: 6px;
    }

    .bottombarRight div {
        cursor: pointer;
    }

    .bottombarRight i {
        font-size: 12px !important;
    }

    .bottombarLeft {
        overflow: hidden;
        text-overflow: ellipsis;
        padding-left: 8px;
    }

    .icon {
        font-size: 10px !important;
    }

    #donateQrCode {
        color: #555;
    }

    #donateQrCode .tip {
        color: #888;
        font-size: 12px;
        line-height: 28px;
    }

    #donateQrCode img {
        width: 198px;
        height: 198px;
        cursor: pointer;
    }

    #loginQrCode {
        height: 460px;
        width: 456px;
    }
</style>