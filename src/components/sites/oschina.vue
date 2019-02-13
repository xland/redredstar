<template>
    <div :style="showWebview?'z-index:99;':'z-index:1;'">
        <iframe id="oschinaFrame" @load="frameLoad" nwdisable nwfaketop :src="url" :nwUserAgent="$root.agent"></iframe>
        <div style="display: none">
            <div id="oscClasses">
                请选择文章分类<br /><select id='oscSelect'></select>
            </div>
        </div>
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
        computed: {
            article() {
                return this.$root.a[this.$root.aIndex];
            }
        },
        mounted() {
            this.showWebview = true;
        },
        methods: {
            loadUrl() {
                if (this.article.oschina) {
                    this.url = 'https://my.oschina.net/u/' + this.article.oschina.userId + '/blog/write/' + this.article.oschina.articleId;
                } else {
                    this.url = 'https://www.oschina.net/blog';
                }
            },
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
                    if (curUrl == 'https://www.oschina.net/blog' || curUrl.startsWith(
                            'https://www.oschina.net/?nocache=')) {
                        var userId = $(".avatar[data-user-id]")[0].dataset.userId;
                        self.article.oschina = {};
                        self.article.oschina.userId = userId;
                        self.frame.location.href = "https://my.oschina.net/u/" + userId + "/blog/write";
                        return;
                    }
                    if (curUrl.startsWith('https://my.oschina.net/u/' + self.article.oschina.userId +
                        '/blog/write')) {
                        var html = $("select[name='classification']").html().replace(' ', '');
                        document.getElementById("oscSelect").innerHTML = html;
                        swal({
                            content: document.getElementById("oscClasses"),
                            buttons: {
                                cancel: {
                                    visible: false,
                                },
                                confirm: {
                                    text: "OK",
                                    value: true,
                                    visible: true,
                                }
                            }
                        }).then((value) => {
                            if (!value) return;
                            var classId = document.getElementById("oscSelect").value;
                            $("select[name='classification']").val(classId);
                            self.uploadImgs();
                        });
                        return;
                    }
                    if ($("h2.header").text().trim().includes(self.article.title)) {
                        var id = $(".article-like")[0].dataset.id;
                        self.article.oschina.articleId = id;
                        self.$root.needSave.a = true;
                        self.$root.needSave.c = true;
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
            uploadOneImg(file, id) {
                var self = this;
                var token = self.frame.CKEDITOR.tools.getCsrfToken();
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
                        var imgDom = editorDocument.getElementById(id);
                        imgDom.dataset.img_oschina = imgObj.url;
                        self.uploadImgNum -= 1;
                        if (self.uploadImgNum <= 0) {
                            self.publish();
                        }
                    }
                };
                xhr.send(formData);
            },
            uploadImgs() {
                var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
                var imgs = editorDocument.getElementsByTagName("img");
                var justPublish = true;
                for (var i = 0; i < imgs.length; i++) {
                    var item = imgs[i];
                    if (!item.dataset.oschina && item.src.startsWith('file://')) {
                        this.$parent.publishText = "OSC：正在上传图片..."
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
                this.$parent.publishText = "OSC：正在发布文章...";
                var html = this.$parent.prepareImgSrc('oschina');
                this.frame.$("input[name='title']").val(this.article.title);
                this.frame.CKEDITOR.instances["body"].setData(html);
                var self = this;
                setTimeout(function(){
                    self.frame.$(".primary.large.submit.button").click();
                },518);
            }
        }
    }
</script>