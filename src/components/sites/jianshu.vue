<template>
    <div class="webview" :style="showWebview?'z-index:99;':'z-index:1;'">
        <iframe id="jianshuFrame" @load="frameLoad" nwdisable nwfaketop :src="url" :nwUserAgent="$root.agent"></iframe>
    </div>
</template>
<script>
    var fs = nw.require('fs');
    var path = nw.require('path');
    import swal from 'sweetalert';
    export default {
        data() {
            return {
                url: 'https://www.jianshu.com/writer#/',
                showWebview: false,
                uploadImgNum: 0,
                frame: null,
                jqueryData: ''
            }
        },
        computed: {
            article() {
                return this.$root.a[this.$root.aIndex];
            }
        },
        created() {
            var jqueryPath = path.join(window.process.mainModule.filename, '/ui/static/tools/jquery-3.3.1.min.js');
            this.jqueryData = fs.readFileSync(jqueryPath, {
                encoding: "utf8"
            });
        },
        methods: {
            createNote() {
                var self = this;
                var getBookUrl = 'https://www.jianshu.com/author/notebooks';
                var addNoteUrl = "https://www.jianshu.com/author/notes"
                self.frame.$.get(getBookUrl, {}, function (data) {
                    var bookId = data[0].id.toString(); //默认分类
                    self.frame.$.ajax({
                        type: "POST",
                        url: addNoteUrl,
                        contentType: "application/json; charset=utf-8",
                        data: JSON.stringify({
                            notebook_id: bookId,
                            title: new Date().format('yyyy-MM-dd'),
                            at_bottom: false
                        }),
                        dataType: "json",
                        success: function (data) {
                            self.article.jianshu = {
                                bookId,
                                noteId: data.id.toString(),
                                autosave_control: 1, //版本号
                                slug: data.slug //发布后的文章ID
                            };
                            self.uploadImgs();
                        }
                    });
                })
            },
            injectJq() {
                var self = this;
                var dom = self.frame.document.createElement("script");
                dom.innerHTML = self.jqueryData;
                self.frame.document.body.appendChild(dom);
                self.frame.$.ajaxSetup({
                    error: function (r) {
                        if (r.responseText == '继续操作前请注册或者登录.' || r.responseText ==
                            '{"error":"继续操作前请注册或者登录."}') {
                            self.frame.location.href = "https://www.jianshu.com/sign_in";
                            return;
                        }
                        swal({
                            icon: "error",
                            text: "请求异常，请联系作者",
                        });
                        self.$parent.$parent.showSites = false;
                        return;

                    }
                });
            },
            uploadOneImg(file, domId) {
                var self = this;
                self.frame.$.get("https://www.jianshu.com/upload_images/token.json?filename=" + file.name, {}, function (
                    r) {
                    var formData = new FormData();
                    formData.append("token", r.token);
                    formData.append("key", r.key);
                    formData.append("file", file);
                    formData.append("x:protocol", 'https');
                    var xhr = new XMLHttpRequest();
                    xhr.open("POST", 'https://upload.qiniup.com/', true);
                    xhr.setRequestHeader("Cache-Control", "no-cache");
                    xhr.onreadystatechange = function () {
                        if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                            var imgObj = JSON.parse(xhr.responseText);
                            var editorDocument = document.getElementById("ueditor_0").contentWindow
                                .document;
                            var imgDom = editorDocument.getElementById(domId);
                            imgDom.dataset.img_jianshu = imgObj.url;
                            self.uploadImgNum -= 1;
                            if (self.uploadImgNum <= 0) {
                                self.postNote();
                            }
                        }
                    };
                    xhr.send(formData);
                })
            },
            uploadImgs() {
                var editorDocument = document.getElementById("ueditor_0").contentWindow.document;
                var imgs = editorDocument.getElementsByTagName("img");
                var justPublish = true;
                for (var i = 0; i < imgs.length; i++) {
                    var item = imgs[i];
                    if (!item.dataset.img_jianshu && item.src.startsWith('file://')) {
                        this.$parent.publishText = "简书：正在上传图片..."
                        var filePath = decodeURI(item.src.substr(7));
                        var fileName = path.basename(filePath);
                        var file = new File(filePath, fileName);
                        this.uploadImgNum += 1;
                        this.uploadOneImg(file, item.id);
                        justPublish = false;
                    }
                }
                if (justPublish) {
                    this.postNote();
                }
            },
            postNote() {
                var self = this;
                self.$parent.publishText = "简书：正在发布文章...";
                var html = self.$parent.prepareImgSrc('jianshu');
                self.frame.$.ajax({
                    type: "PUT", //保存文章
                    url: "https://www.jianshu.com/author/notes/" + self.article.jianshu.noteId,
                    contentType: "application/json; charset=utf-8",
                    data: JSON.stringify({
                        id: self.article.jianshu.noteId,
                        autosave_control: self.article.jianshu.autosave_control,
                        title: self.article.title,
                        content: html
                    }),
                    dataType: "json",
                    success: function (data) {
                        //todo: 如果文章被浏览器编辑过，那么这个版本号可能会改变
                        self.article.jianshu.autosave_control += 1;
                        self.frame.$.ajax({
                            type: "POST", //发布文章
                            url: 'https://www.jianshu.com/author/notes/' + self.article.jianshu
                                .noteId + '/publicize',
                            contentType: "application/json; charset=utf-8",
                            dataType: "json",
                            success: function (publicizeData) {
                                self.$root.needSave.a = true;
                                self.$root.needSave.c = true;
                                self.$root.save();
                                self.$parent.$parent.showSites = false;
                                self.$parent.publishText = "简书：发布成功！";
                                swal({
                                    icon: "success",
                                    text: "简书：知识发布成功,现在查看？",
                                    buttons: [
                                        '取消', "带我去"
                                    ]
                                }).then((value) => {
                                    if (!value) return;
                                    var url = 'https://www.jianshu.com/p/' + self.article
                                        .jianshu.slug;
                                    window.nw.Shell.openExternal(url);
                                });
                                return;
                            }
                        })
                    }
                });
            },
            frameLoad() {
                var self = this;
                self.frame = document.getElementById('jianshuFrame').contentWindow;
                var url = self.frame.location.href;
                if (url == 'https://www.jianshu.com/sign_in') {
                    self.showWebview = true;
                    return;
                }
                self.showWebview = false;
                if (!url.startsWith('https://www.jianshu.com/writer#/')) {
                    self.frame.location.href = self.url;
                    return;
                }
                self.injectJq();
                if (!self.article.jianshu) {
                    self.createNote();
                } else {
                    self.uploadImgs();
                }
            }
        }
    }
</script>