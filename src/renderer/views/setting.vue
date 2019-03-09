<template>
    <div v-if="setting" class="setting view">
        <div class="formItem">
            <div>自动保存文章的时间间隔：</div>
            <div>
                <input @change="autoSaveIntervalSeconds" v-model="setting.autosave_interval" type="number" />
            </div>
            <div>
                秒
            </div>
        </div>
        <div class="formItem">
            <div>文章内图片长超过</div>
            <div>
                <input @change="compressHeight" v-model="setting.img_h" type="number" />
            </div>
            <div>
                px，且宽超过
            </div>
            <div>
                <input @change="compressWidth" v-model="setting.img_w" type="number" />
            </div>
            <div>
                px，将启用图片等比例压缩（任一值设置为-1将禁用图片压缩）
            </div>
        </div>
        <div class="formItem">
            默认编辑器：
            <div @click="setting.editor_type='html'" class="rdBtn">
                <i :class="['iconfont',setting.editor_type=='html'?'icon-xuanzhong':'icon-weixuanzhong']"></i>
                HTML
            </div>
            <div @click="setting.editor_type='markdown'" class="rdBtn">
                <i :class="['iconfont',setting.editor_type=='markdown'?'icon-xuanzhong':'icon-weixuanzhong']"></i>
                MarkDown
            </div>
        </div>
        <div class="formItem">
            <div @click="save" class="btn" style="margin-left: 0px;">保存</div>
        </div>
        <div style="color: #888;">
            <div style="line-height: 36px;font-size: 16px;margin-top: 12px;">系统说明</div>
            <div style="line-height: 26px;">
                 您通过“想学吗”编辑的知识，以及知识内部的图片、个人设置等数据均保存在本地；
                <br /> 您的知识，可以自由的发布到“微信公众号”、“简书”、“博客园”、“开源中国”等知名网站（需拥有相应网站的账号）；
                <br /> 发布知识时，知识内部的图片也会上传到对应的网站上；
                <br /> 修改知识后，再次发布该知识，不会导致图片重复上传；
                <br /> 在文章中删除图片，本地目录中的图片也会被删除，不会留有垃圾数据；
                <br /> 到目前为止“想学吗”不会保存您的任何账号数据；
                <br /> 文章发布到目标平台不夹带任何“尾巴”
            </div>
        </div>
    </div>
</template>
<script>
    import swal from 'sweetalert';
    export default {
        data() {
            return {
                setting: null
            }
        },
        mounted() {
            this.db("settings").select("*").then(rows => {
                this.setting = rows[0];
            })
        },
        methods: {
            save() {
                this.db("settings")
                    .update(this.setting)
                    .where("id", this.setting.id)
                    .then(() => {
                        this.alert("保存成功，重启后生效", "success")
                    });
            },
            alert(str, iconType = 'error') {
                swal({
                    icon: iconType,
                    text: str,
                });
            },
            autoSaveIntervalSeconds() {
                if (this.setting.autosave_interval < 3) {
                    this.setting.autosave_interval = 3;
                    this.alert("自动保存时间间隔不能小于3秒");
                }
            },
            compressWidth() {
                if (this.setting.img_w < 500 && this.setting.img_w > 0) {
                    this.setting.img_w = 500;
                    this.alert("最小宽度不能小于500");
                }
            },
            compressHeight() {
                if (this.setting.img_h < 300 && this.setting.img_h > 0) {
                    this.setting.img_h = 300;
                    this.alert("最小高度不能小于300");
                }
            }
        }
    }
</script>
<style>
    .setting {
        overflow: hidden;
        margin: 8px;
        margin-top: 0px;
        box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
        height: calc(100% - 24px);
        border-radius: 3px;
        display: flex;
        flex-flow: column;
        background: #fff;
        padding: 8px;
    }

    .formItem {
        height: 36px;
        line-height: 36px;
        margin-left: 12px;
        color: #363636;
    }

    .formItem input {
        border: 1px solid #eee;
        border-radius: 3px;
        line-height: 22px;
        height: 22px;
        outline: none;
        text-align: center;
        width: 50px;
        font-size: 12px;
    }

    .formItem div {
        display: inline-block;
    }

    .formItem div:last-child {
        padding-left: 8px;
    }

    .rdBtn {
        cursor: pointer;
    }

    .icon-xuanzhong {
        color: #1787fb !important;
    }
</style>