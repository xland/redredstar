<template>
    <div :style="showWebview?'z-index:99;':'z-index:1;'">
        <iframe id="zhihuFrame" @load="frameLoad" nwdisable nwfaketop :src="url" :nwUserAgent="$root.agent"></iframe>
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
                if (this.article.zhihu) {
                    this.url = 'https://my.zhihu.net/u/' + this.article.zhihu.userId + '/blog/write/' + this.article.zhihu
                        .articleId;
                } else {
                    this.url = 'https://zhuanlan.zhihu.com/write';
                }
            },
            frameLoad() {
                var self = this;
                self.frame = document.getElementById('zhihuFrame').contentWindow;
                var curUrl = self.frame.location.href;
                if (curUrl.startsWith("https://www.zhihu.com/signin")) {
                    self.showWebview = true;
                    return;
                }
                if (curUrl == 'https://zhuanlan.zhihu.com/write') {
                    this.uploadImgs();
                    return;
                }
            },
            uploadOneImg(file, id) {
                var self = this;
                var formData = new FormData();
                formData.append('picture', file);
                formData.append("source", "article");
                var xhr = new XMLHttpRequest();
                xhr.open("POST", 'https://zhuanlan.zhihu.com/api/uploaded_images', true);
                xhr.setRequestHeader("X-Requested-With", "Fetch");
                xhr.setRequestHeader("accept", "application/json, text/plain, */*");
                xhr.onreadystatechange = function () {
                    if (xhr.readyState == 4 && (xhr.status == 200 || xhr.status == 304)) {
                        var imgObj = JSON.parse(xhr.responseText);
                        var editorDocument = document.getElementById("ueditor_0").contentWindow
                            .document;
                        var imgDom = editorDocument.getElementById(id);
                        imgDom.dataset.img_zhihu = imgObj.src;
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
                    if (!item.dataset.zhihu && item.src.startsWith('file://')) {
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
                    this.publish();
                }
            },
            publish() {
                this.$parent.publishText = "简书：正在发布文章...";
                var html = this.$parent.prepareImgSrc('zhihu');
                var d = this.frame.document;
                d.getElementsByClassName("WriteIndex-titleInput")[0].children[0].value = this.article.title
                d.getElementsByClassName("public-DraftEditor-content")[0].children[0].innerHTML = html;
                var self = this;
                setTimeout(function () {
                    d.getElementsByClassName("PublishPanel-triggerButton")[0].click();
                }, 518);
            }
        }
    }
</script>