<template>
  <div @click="$parent.showSites = false" class="maskExceptMenu">
    <div class="tarSiteContainer">
      <div
        @click.stop
        @mouseenter="overOneSite(index,item)"
        @mouseleave="overIndex = -1"
        :class="['tarSiteItem',item.ready?'':'notReady']"
        :key="item.id"
        v-for="(item,index) in sites"
      >
        <div class="tarSiteIcon">
          <img :src="'./static/sites/'+item.id+'/logo.png'" />
        </div>
        <div class="tarSiteName">
          <div v-show="overIndex != index || !item.ready" class="toolText">{{item.title}}</div>
          <div
            @click.stop="publish(item,'edit')"
            v-show="editUrl && overIndex == index && item.ready"
            class="toolBtn"
          >修改</div>
          <div
            @click.stop="publish(item,'new')"
            v-show="overIndex == index && item.ready"
            class="toolBtn"
          >新增</div>
        </div>
      </div>
    </div>
  </div>
</template>
<script>
var fs = require("fs");
var path = require("path");
import swal from "sweetalert";
import sites from "../utils/site";
const { BrowserWindow } = require("electron").remote;
export default {
  data() {
    return {
      overIndex: -1,
      editUrl: null,
      winOption: {
        width: 1056,
        height: 680,
        webPreferences: {
          nodeIntegration: false,
          preload: null
        }
      },
      urlOption: {
        userAgent:
          "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.98 Safari/537.36"
      },
      initWebview: false,
      sites: sites
    };
  },
  methods: {
      //todo:鼠标滑过按钮时，取出修改路径，有必要这样做吗？
    async overOneSite(index, item) {
      this.overIndex = index;
      this.editUrl = null;
      let row = await this.db("article_site")
        .where({
          article_id: this.$parent.article.id,
          site_id: item.id
        })
        .first();
      if (row) this.editUrl = row.edit_url;
      return false;
    },
    makeWin(item, articleMsg) {
      let win = null;
      if (item.winId) {
        win = BrowserWindow.fromId(item.winId);
        win.focus();
        win.webContents.send("message", articleMsg);
        return;
      }
      win = new BrowserWindow(this.winOption);
      item.winId = win.id;
      articleMsg.winId = win.id;
      win.on("closed", () => {
        item.winId = null;
        win = null;
      });
      win.loadURL(articleMsg.url, this.urlOption);
      let self = this;
      win.webContents.on("dom-ready", () => {
        win.webContents.send("message", articleMsg);
      });
    },
    publish(item, type) {
      let content = "";
      if (this.$parent.article.editor_type == "html") {
        content = window.CKEDITOR.instances.editorCk.getData();
      } else {
        content = window.editorMd.getHtml();
      }
      let articleMsg = {
        title: this.$parent.article.title,
        id: this.$parent.article.id,
        winId: item.winId,
        siteId: item.id,
        url: type == "new" ? item.url : this.editUrl,
        type,
        content
      };
      this.winOption.webPreferences.preload = path.join(
        __static,
        "sites/" + item.id + "/inject.js"
      );
      this.makeWin(item, articleMsg);
    }
  }
};
</script>
<style lang="scss" scoped>
.tarSiteContainer {
  text-align: center;
  margin-top: 60px;
  margin-left: 38px;
  margin-right: 38px;
  display: flex;
  flex-wrap: wrap;
  align-content: center;
  justify-content: center;
}
.tarSiteItem {
  width: 96px;
  min-width: 96px;
  height: 88px;
  background: #fff;
  overflow: hidden;
  border-radius: 3px;
  margin: 12px;
}

.tarSiteItem:hover {
  box-shadow: 0 3px 6px rgba(16, 16, 16, 0.6);
}

.tarSiteIcon {
  height: 56px;
  line-height: 56px;
  text-align: center;
}

.notReady {
  filter: blur(2px);
  cursor: default;
}

.notReady:hover {
  box-shadow: none;
}

.tarSiteIcon img {
  margin-top: 6px;
  height: 30px;
  width: 30px;
  vertical-align: middle;
}

.tarSiteName {
  height: 32px;
  line-height: 32px;
  text-align: center;
  color: #555;
  display: flex;
}

.toolText {
  flex: 1;
  text-align: center;
}

.toolBtn {
  flex: 1;
}

.toolBtn:hover {
  border-radius: 3px;
  background: #e7f3ff;
  color: #007acc;
  cursor: pointer;
}
</style>