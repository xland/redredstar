<template>
    <div class="titleBar">
        <div class="right">
            <div @click="setting" class="leftBtn settingBtn" title="系统设置">
                <i class="iconfont icon-setting"></i>
            </div>
            <div v-if="!isMac" @click="min()" class="leftBtn min">
                <i class="iconfont icon-min tool-i"></i>
            </div>
            <div v-if="!isMac" @click='max()' class="leftBtn max">
                <i class="iconfont icon-mac-max tool-i"></i>
            </div>
            <div v-if="!isMac" @click="close()" class="leftBtn close">
                <i class="iconfont icon-guanbi tool-i"></i>
            </div>
        </div>
        <div class="left" v-if="isMac">
            <div @click="close()" class="leftBtn close">
                <i class="iconfont icon-guanbi tool-i"></i>
            </div>
            <div @click="min()" class="leftBtn min">
                <i class="iconfont icon-min tool-i"></i>
            </div>
            <div @click='max()' class="leftBtn max">
                <i class="iconfont icon-mac-max tool-i"></i>
            </div>
        </div>
        <div class="title">
            想学吗「个人知识管理工具」
        </div>
    </div>
</template>
<script>
    export default {
        data() {
            return {
                isMac: true,
            }
        },
        created() {
            this.isMac = window.nw.require("os").platform == "darwin";
        },
        methods: {
            setting() {
                this.bus.$emit('findOrAddTab', {
                    url: '/setting',
                    text: "系统设置",
                });
            },
            close() {
                window.nw.App.quit(); //todo:
                //window.nw.Window.get().close();
            },
            min() {
                window.nw.Window.get().minimize();
            },
            max() {
                window.nw.Window.get().maximize();
            }
            //todo:设置，neditor要精简
        }
    }
</script>
<style lang="scss" scoped>
    .left {
        float: left;
        padding-left: 8px;
        -webkit-app-region: no-drag;
    }

    .right {
        float: right;
        padding-right: 3px;
        -webkit-app-region: no-drag;
        width: 120px;
        text-align: right;
    }

    .titleBar {
        height: 24px;
        background: #f6f6f6;
        line-height: 24px;
        font-size: 12px;
        color: #666;
        border-bottom: 1px solid #e6e6e6;
        -webkit-app-region: drag;
    }

    .titleBar .title {
        margin-left: 180px;
        margin-right: 180px;
        text-align: center;
    }

    .titleBar .leftBtn {
        margin-top: 6px;
        height: 11px;
        line-height: 11px;
        padding-top: 1px;
        width: 12px;
        border-radius: 6px;
        text-align: center;
        display: inline-block;
        margin-right: 6px;
    }

    .titleBar .leftBtn:hover {
        color: #333;
    }

    .titleBar .close {
        background: #fc5b57;
        color: #fc5b57;
    }

    .titleBar .min {
        background: #e5bf3c;
        color: #e5bf3c;
    }

    .titleBar .max {
        background: #34c84a;
        color: #34c84a;
    }
    .right i {
        color: #333 !important;
    }

    .tool-i {
        font-size: 12px !important;
    }
</style>