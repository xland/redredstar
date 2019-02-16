<template>
    <div class="setting view">
        <div class="formItem">
            <div>自动保存文章的时间间隔：</div>
            <div>
                <input @change="autoSaveIntervalSeconds" v-model="$root.u.autoSaveIntervalSeconds" type="number" />
            </div>
            <div>
                秒
            </div>
        </div>
        <div style="color: #888;">
            <div style="line-height: 36px;font-size: 16px;margin-top: 12px;">系统说明</div>
            <div style="line-height: 26px;">
                您通过“想学吗”编辑的知识，以及知识内部的图片、个人设置等数据均保存在本地；<br />
                您通过“想学吗”编辑的知识，可以自由的发布到“简书”、“博客园”等知名网站；<br />
                发布知识时，知识内部的图片也会上传到对应的网站上；<br />
                修改知识后，再次发布该知识，不会导致图片重复上传；<br />
                “想学吗”不会保存您的任何账号数据；<br />
                本软件为开源、免费软件，任何责任与义务均由使用者承担，与开发者无关；<br />
            </div>
        </div>
    </div>
</template>
<script>
    import swal from 'sweetalert';
    export default {
        data() {
            return {

            }
        },
        methods: {
            autoSaveIntervalSeconds: function () {
                if (this.$root.u.autoSaveIntervalSeconds < 6) {
                    this.$root.u.autoSaveIntervalSeconds = 6;
                    swal({
                        icon: "error",
                        text: "自动保存时间间隔不能小于6秒",
                    });
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
    }

    .formItem input {
        border: 1px solid #eee;
        border-radius: 3px;
        line-height: 22px;
        height: 22px;
        outline: none;
        text-align: center;
        width: 60px;
    }

    .formItem div {
        display: inline-block;
    }

    .formItem div:last-child {
        padding-left: 8px;
    }
</style>