<template>
  <div class="editor">
    <div v-show="$parent.article.editor_type == 'markdown'" class="tip">请不要改变img标签的格式（会影响文章发布）</div>
    <div v-show="$parent.article.editor_type == 'markdown'" id="editorMd"></div>
    <textarea v-show="$parent.article.editor_type == 'html'" id="editorCk">{{articleContent}}</textarea>
  </div>
</template>
<script>
//todo:图片放大缩小需要按比例
//todo  黏贴过来的文章，下载图片；
//todo 新建知识右边来一个箭头，可以选编辑器
const fs = require("fs");
const path = require("path");
import img from "./mixins/img";
import md from "./mixins/md";
import ck from "./mixins/ck";
import jna from "./mixins/jna";
import { Promise, reject } from "bluebird";
const { ipcRenderer, remote } = require("electron");
const log = require('electron-log');
export default {
  mixins: [img, md, ck, jna],
  data() {
    return {
      articleContent: null,
      articlePath: null,
      tick: null,
      needSave: false
    };
  },
  methods: {
    saveContent() {
      return new Promise(async (resolve, reject) => {
        if (!this.needSave) {
          resolve();
          return;
        }
        this.bus.$emit("saving");
        if (this.$parent.article.editor_type == "html") {
          this.articleContent = window.CKEDITOR.instances.editorCk.getData();
        } else {
          this.articleContent = window.editorMd.getValue();
        }
        // log.info(`saving:id:${this.$parent.article.id}; title:${this.$parent.article.title}; content:${this.articleContent.substr(0,20)}`);
        fs.writeFileSync(
          path.join(this.articlePath, "a.data"),
          this.articleContent,
          this.$root.rwOption
        );
        await this.db("articles")
          .update({
            title: this.$parent.article.title,
            updated_at: new Date()
          })
          .where("id", this.$parent.article.id);
        this.needSave = false;
        // log.info(`saved:id:${this.$parent.article.id};  title:${this.$parent.article.title}; content:${this.articleContent.substr(0,20)}`);
        resolve();
      });
    },
    focus() {
      if (this.$parent.article.editor_type == "html") {
        window.CKEDITOR.instances.editorCk.focus();
      } else {
        window.editorMd.focus();
      }
    },
    destroy() {
      clearInterval(this.tick);
      if (
        this.$parent.article.editor_type == "html" &&
        CKEDITOR.instances.editorCk
      ) {
        CKEDITOR.instances.editorCk.destroy();
      }
    },
    hookImgUpload() {
      this.$root.imgUploadCb = obj => {
        if (this.$parent.article.editor_type == "html") {
          this.imageUploadCk(obj);
        } else {
          this.imageUploadMd(obj);
        }
        this.needSave = true;
      };
    },
    hookWinQuit() {
      var self = this;
      remote.getCurrentWindow().on("close", async event => {
        event.preventDefault();
        await self.saveContent();
        remote.app.quit();
      });
    },
    getContent() {
      this.articlePath = path.join(
        remote.app.getPath("userData"),
        "/xxm/" + this.$parent.article.id
      );
      this.articleContent = fs.readFileSync(
        path.join(this.articlePath, "a.data"),
        this.$root.rwOption
      );
      // log.info(
      //   `entered:id:${this.$parent.article.id};  content:${this.articleContent.substr(0, 20)}`
      // );
      if (this.$parent.article.editor_type == "html") {
        this.$nextTick(() => {
          this.initEditorCk();
        });
      } else {
        this.$nextTick(() => {
          this.initEditorMd();
        });
      }
      this.tick = setInterval(
        async _ => await this.saveContent(),
        this.$root.tickStep
      );
      this.hookImgUpload();
      this.removeUselessImg();
    }
  },
  mounted() {
    this.hookWinQuit();
  }
};
</script>
<style scoped lang="scss">
.editor {
  overflow: hidden;
  flex: 1;
  background: #fff;
  border-top: 1px solid #e5e5e5;
  border-bottom: 1px solid #e5e5e5;
}

.mdSelected {
  background: #fff !important;
}

@keyframes flash {
  0% {
    color: red;
  }

  20% {
    color: #ccc;
  }

  40% {
    color: red;
  }

  60% {
    color: #ccc;
  }

  80% {
    color: red;
  }

  100% {
    color: #ccc;
  }
}

.tip {
  animation: flash 5.6s linear;
  position: absolute;
  z-index: 6;
  line-height: 32px;
  right: 12px;
  color: #ccc;
}
</style>