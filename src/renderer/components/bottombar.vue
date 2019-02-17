<template>
    <div class="bottombar">
        <div class="bottombarRight">
            <div @click="setting">
                <i class="iconfont icon-shezhi"></i> Setting
            </div>
            <div @click="addBug">
                <i class="iconfont icon-bug icon"></i> Bug
            </div>
            <div @click="addDonate">
                <i class="iconfont icon-juanzeng icon"></i> Donate
            </div>
            <div>
                <i class="iconfont icon-version icon"></i> {{getVersion()}}
            </div>
        </div>
        <div class="bottombarLeft">
            <div>
                <i class="iconfont icon-wenzhang icon"></i>
                {{$root.a.length}}
            </div>
            <div>
                <i class="iconfont icon-biaoqian icon"></i>
                {{$root.t.length}}
            </div>
            <!-- <div>
                <i class="iconfont icon-zishu icon"></i>
            </div> -->
            <div :class="rotating&&$root.aIndex>=0?'rotating':''">
                <i class='iconfont icon-baocun icon'></i>
            </div>
        </div>
        <div style="display: none">
            <div id="donatePics">
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
        watch: {
            "$root.needSave": {
                handler: function (val, oldVal) {
                    if (!val.a && !val.c && !val.t && !val.u) {
                        this.rotating = true;
                        var self = this;
                        setTimeout(function () {
                            self.rotating = false;
                        }, 600);
                    }
                },
                deep: true
            },
        },
        methods: {
            setting(){
                this.bus.$emit('findOrAddTab', {
                    url: '/setting',
                    text: "系统设置",
                });
            },
            addDonate() {
                swal({
                    content: document.getElementById("donatePics"),
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
    .bottombarRight i{
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

    #donatePics {
        color: #555;
    }

    #donatePics .tip {
        color: #888;
        font-size: 12px;
        line-height: 28px;
    }

    #donatePics img {
        width: 198px;
        height: 198px;
        cursor: pointer;
    }
</style>