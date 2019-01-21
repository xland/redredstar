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
                this.url = 'https://my.oschina.net/u/' + this.$root.article.oschina.userId + '/blog/write/' + this.$root
                    .article.oschina.articleId;
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
                        self.uploadImgs();
                    }
                    var reg = new RegExp('^https:\/\/my\.oschina\.net\/u\/' + self.article.oschina.userId +
                        '\/blog\/\d+\/{0,1}$', 'gi')
                    if (reg.test(curUrl)) {
                        var id = $(".article-like")[0].dataset.id;
                        self.$root.article.oschina.articleId = id;
                        self.$root.save();
                        self.$parent.$parent.showSites = false;
                        self.$parent.publishText = "OSC：发布成功！";
                        swal({
                            icon: "success",
                            text: "OSC：知识发布成功,现在查看？",
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
                    if ($(".error-page-wrap #errorIcon")) {
                        self.$parent.$parent.showSites = true;
                    }
                });
            },
            uploadOneImg(file, id, token) {
                var self = this;
                var formData = new FormData();
                formData.append('upload', file);
                formData.append("ckCsrfToken", token);
                var xhr = new XMLHttpRequest();
                xhr.open("POST", 'https://my.oschina.net/u/1432189/space/ckeditor_dialog_img_upload', true);
                xhr.setRequestHeader("Cache-Control", "no-cache");
                xhr.onreadystatechange = function () {
                    if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                        var imgObj = JSON.parse(xhr.responseText);
                        var editorDocument = document.getElementById("ueditor_0").contentWindow
                            .document;
                        var imgDom = editorDocument.getElementById(domId);
                        imgDom.dataset.img_oschina = imgObj.url;
                        self.uploadImgNum -= 1;
                        if (self.uploadImgNum <= 0) {
                            self.postNote();
                        }
                    }
                };
                xhr.send(formData);
            },
            uploadImgs() {
                var token = self.frame.CKEDITOR.tools.getCsrfToken()
                var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
                var imgs = editorDocument.getElementsByTagName("img");
                var justPublish = true;
                for (var i = 0; i < imgs.length; i++) {
                    var item = imgs[i];
                    if (!item.dataset.oschina && item.src.startsWith('file://')) {
                        this.$parent.publishText = "博客园：正在上传图片..."
                        var filePath = decodeURI(item.src.substr(7));
                        var fileName = path.basename(filePath);
                        var file = new File(filePath, fileName);
                        this.uploadImgNum += 1;
                        this.uploadOneImg(file, item.id, token);
                        justPublish = false;
                    }
                }
                if (justPublish) {
                    this.publish();
                }
            },
            publish() {
                this.$root.article.content = window.UE.instants.ueditorInstant0.getContent();
                this.$parent.publishText = "OSC：正在发布文章...";
                var html = this.$parent.prepareImgSrc('oschina');
                this.frame.$("input[name='title']").val(this.$root.article.title);
                this.frame.CKEDITOR.instances["body"].setData(html);
                this.frame.$(".primary.large.submit.button").click();
            }
        }
    }
</script>