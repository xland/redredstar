<template>
    <div :style="showWebview?'z-index:99;':'z-index:1;'">
        <iframe id="oschinaFrame" @load="frameLoad" nwdisable nwfaketop :src="url" :nwUserAgent="$root.agent"></iframe>
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
        loadUrl() {
            if (this.$root.article.oschina) {
                this.url = 'https://my.oschina.net/u/'+this.$root.article.oschina.userId+'/blog/write/' + this.$root.article.oschina.articleId;
            } else {
                this.url = 'https://www.oschina.net/blog';
            }
        },
        mounted() {
            this.showWebview = true;
        },
        methods: {
            frameLoad() {
                var self = this;
                self.frame = document.getElementById('oschinaFrame').contentWindow;
                self.frame.$(function () {
                    var curUrl = self.frame.location.href;
                    var $ = self.frame.$;
                    if ($("#headerNavMenu .item[href^='https://www.oschina.net/home/login']").length > 0) {
                        self.frame.location.href = "https://www.oschina.net/home/login";
                        return;
                    }
                    if (curUrl.startsWith('https://www.oschina.net/?nocache=')) {
                        var userId = $(".avatar[data-user-id]")[0].dataset.userId;
                        self.article.oschina = {};
                        self.article.oschina.userId = userId;
                        self.frame.location.href = "https://my.oschina.net/u/" + userId + "/blog/write";
                        return;
                    }
                    if (curUrl == 'https://my.oschina.net/u/' + self.article.oschina.userId + '/blog/write') {
                        //CKEDITOR.instances.body.insertHtml("allen")
                    }
                    var reg = new RegExp('^https:\/\/my\.oschina\.net\/u\/' + self.article.oschina.userId +
                        '\/blog\/\d+\/{0,1}$', 'gi')
                    if (reg.test(curUrl)) {
                        var id = $(".article-like")[0].dataset.id;
                        self.$root.article.oschina.articleId = id;
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
                            var url = 'https://my.oschina.net/u/' + self.article.oschina.userId +
                                '/blog/' + id;
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