<template>
    <div :style="showWebview?'z-index:99;':'z-index:1;'">
        <iframe id="cnblogsFrame" @load="frameLoad" nwdisable nwfaketop :src="url" :nwUserAgent="$root.agent"></iframe>
    </div>
</template>
<script>
    var path = nw.require('path');
    import swal from 'sweetalert';
    export default {
        data() {
            return {
                url: '',
                showWebview: false,
                uploadImgNum: 0,
                frame: null,
            }
        },
        created() {
            this.loadUrl();
        },
        mounted() {
        },
        methods: {
            loadUrl() {
                if (this.$root.article.cnblogsId) {
                    this.url = 'https://i.cnblogs.com/EditPosts.aspx?postid=' + this.$root.article.cnblogsId;
                } else {
                    this.url = 'https://i.cnblogs.com/EditPosts.aspx?opt=1';
                }
            },
            frameLoad() {
                var self = this;
                self.frame = document.getElementById('cnblogsFrame').contentWindow;
                self.frame.$(function () {
                    var curUrl = self.frame.location.href;
                    if (curUrl.startsWith('https://passport.cnblogs.com/user/signin')) {
                        self.showWebview = true;
                        return;
                    }
                    if (curUrl.startsWith('https://i.cnblogs.com/EditPosts.aspx')) {
                        self.showWebview = false;
                        self.uploadImgs();
                        return;
                    }
                    if (curUrl.startsWith('https://i.cnblogs.com/PostDone.aspx')) {
                        var id = curUrl.getParamVal('postid');
                        self.$root.article.cnblogsId = id;
                        self.$root.save();
                        self.$parent.$parent.showSites = false;
                        self.$parent.publishText = "博客园：发布成功！";
                        swal({
                            icon: "success",
                            text: "博客园：知识发布成功,现在查看？",
                            buttons: [
                                '取消', "带我去"
                            ]
                        }).then((value) => {
                            if (!value) return;
                            var url = 'https://www.cnblogs.com/liulun/p/' + id + '.html';
                            window.nw.Shell.openExternal(url);
                        });
                        return;
                    }
                    if (curUrl == "https://i.cnblogs.com/") {
                        //如果没这篇文章了，那么就新建一篇；
                        self.url = 'https://i.cnblogs.com/EditPosts.aspx?opt=1';
                        return;
                    }
                });
            },
            uploadOneImg(file, id) {
                var formData = new FormData();
                formData.append('imageFile', file);
                formData.append("mimeType", file.type);
                var self = this;
                self.frame.$.ajax({
                    type: 'POST',
                    url: 'https://upload.cnblogs.com/imageuploader/CorsUpload',
                    data: formData,
                    processData: false,
                    contentType: false,
                    dataType: 'json',
                    xhrFields: {
                        withCredentials: true
                    },
                    success: function (result) {
                        var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
                        var imgDom = editorDocument.getElementById(id);
                        imgDom.dataset.img_cnblogs = result.message;
                        self.uploadImgNum -= 1;
                        if (self.uploadImgNum <= 0) {
                            self.publish();
                        }
                    }
                });
            },
            uploadImgs() {
                var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
                var imgs = editorDocument.getElementsByTagName("img");
                var justPublish = true;
                for (var i = 0; i < imgs.length; i++) {
                    var item = imgs[i];
                    if (!item.dataset.img_cnblogs && item.src.startsWith('file://')) {
                        this.$parent.publishText = "博客园：正在上传图片..."
                        var filePath = decodeURI(item.src.substr(7));
                        var fileName = path.basename(filePath);
                        var file = new File(filePath, fileName);
                        this.uploadImgNum += 1;
                        this.uploadOneImg(file, item.id);
                        justPublish = false;
                    }
                }
                if (justPublish) {
                    this.publish();
                }
            },
            publish() {
                this.$root.article.content = window.UE.instants.ueditorInstant0.getContent();
                this.$parent.publishText = "博客园：正在发布文章...";
                var msg = this.frame.$.trim(this.frame.$('Editor_Messages').text());
                if (msg) {
                    swal({
                        icon: "error",
                        text: '博客园反馈：' + msg,
                    });
                    return;
                }
                var html = this.$parent.prepareImgSrc('cnblogs');
                this.frame.$("#Editor_Edit_txbTitle").val(this.$root.article.title);
                this.frame.blogEditor.setContent(html);
                this.frame.$("#Editor_Edit_lkbPost").click();
            }
        }
    }
</script>