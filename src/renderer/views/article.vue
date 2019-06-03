<template>
  <div id="article" class="view" v-if="article">
    <div class="blankLine" style="border-top: 1px solid #e5e5e5;overflow: hidden;">
      <div style="flex: 1;display: flex;overflow: hidden;">
        <input
          autocomplete="off"
          id="articleTitleInput"
          @keydown.tab="titleTab"
          @input="titleChange"
          class="textInput"
          v-model="article.title"
          placeholder="请输入文章标题"
        >
      </div>
      <div class="barBtn" @click="$refs.tagslide.show=true">
        <i class="iconfont icon-biaoqian" style="font-size: 16px !important;"></i>
      </div>
      <div
        class="barBtn"
        @mouseenter="showRecentArticle = true"
        @mouseleave="showRecentArticle = false"
      >
        <i class="iconfont icon-zuijin" style="font-size: 16px !important;"></i>
      </div>
      <div class="barBtn" @click="showSitesClick">
        <i class="iconfont icon-fabu" style="font-size: 16px !important;"></i>
      </div>
      <div class="barBtn" @click="showHelp">
        <i class="iconfont icon-help" style="font-size: 16px !important;"></i>
      </div>
    </div>
    <editor ref="articleEditor"></editor>
    <sites v-show="showSites"></sites>
    <div
      v-show="showRecentArticle"
      class="recent"
      @mouseenter="showRecentArticle = true"
      @mouseleave="showRecentArticle = false"
    >
      <div
        :key="item.id"
        @click="$router.push('/article/' + item.id)"
        v-for="item in recentArticles"
        class="item"
      >{{item.title}}</div>
    </div>
    <tagslide refer="article" :id="$route.params.id" ref="tagslide">
    </tagslide>
    <div style="display: none">
      <div id="helpContainer">
        您通过“想学吗”编辑的知识，以及知识内部的图片、个人设置等数据均保存在本地；
        <br>您的知识，可以自由的发布到“微信公众号”、“简书”、“博客园”、“开源中国”等十几个知名网站（需拥有相应网站的账号）；
        <br>发布知识时，知识内部的图片也会上传到对应的网站上；
        <br>修改知识后，再次发布该知识，不会导致图片重复上传；
        <br>在文章中删除图片，本地目录中的图片也会被删除，不会留有垃圾数据；
        <br>“想学吗”不会保存您任何博客平台的账号数据；
        <br>“想学吗”浏览器插件包括文章收集功能和思想火花收集功能，可以快速收集互联网的文章或简短的文字；
        <br>“想学吗”浏览器插件收集文章时，也会把文章内的图片下载到本地；
        <br>文章发布到自媒体平台，只有保存成草稿，才可以再次编辑修改，发表之后往往是不能修改的；
        <br>
      </div>
    </div>
  </div>
</template>
<script>
import sites from "../components/sites";
import editor from "../components/editor";
import tagslide from "../components/tagslide";
const fs = require("fs");
const path = require("path");
const { remote } = require("electron");
export default {
  components: {
    sites,
    editor,
    tagslide
  },
  data() {
    return {
      showSites: false,
      article: null,
      showRecentArticle: false,
      recentArticles: []
    };
  },
  beforeRouteUpdate(to, from, next) {
    this.showSites = false;
    this.$refs.articleEditor.saveContent(() => {
      this.$refs.articleEditor.destroy();
      next();
    });
  },
  beforeRouteLeave(to, from, next) {
    this.showSites = false;
    this.$refs.articleEditor.saveContent(() => {
      this.$refs.articleEditor.destroy();
      next();
    });
  },
  mounted() {
    let articleId = this.$route.params.id;
    this.getArticle(articleId);
  },
  methods: {
    showHelp() {
      swal({
        content: document.getElementById("helpContainer"),
        buttons: false,
        className: 'modelWidth'
      })
      
    },
    showSitesClick() {
      this.showSites = true;
      if (this.article.editor_type == "html") {
        this.$refs.articleEditor.downloadInternetImg();
      }
    },
    hookArticleRefresh() {
      this.$root.articlePublushCb = obj => {
        this.db("article_site")
          .where("article_id", this.article.id)
          .andWhere("site_id", obj.siteId)
          .select("*")
          .then(rows => {
            let asObj = {
              article_id: this.article.id,
              site_id: obj.siteId,
              edit_url: obj.url
            };
            if (rows.length < 1) {
              this.db("article_site")
                .insert(asObj)
                .then();
            } else {
              this.db("article_site")
                .update(asObj)
                .where("id", rows[0].id)
                .then();
            }
          });
        this.$refs.articleEditor.jnaPublish();
      };
    },
    getArticle(id) {
      this.db("articles")
        .where("id", id)
        .select("*")
        .then(rows => {
          this.article = rows[0];
          this.article.visited_at = new Date();
          this.db("articles")
            .where("id", this.article.id)
            .update(this.article)
            .then(() => {
              this.db("articles")
                .orderBy("visited_at", "desc")
                .limit(8)
                .offset(1)
                .then(recentRows => {
                  this.recentArticles = recentRows;
                });
            });
          this.$nextTick(() => {
            this.$refs.articleEditor.getContent();
            this.hookArticleRefresh();
            window.document.getElementById("articleTitleInput").focus();
          });
        });
    },
    titleTab() {
      setTimeout(() => {
        this.$refs.articleEditor.focus();
      }, 80);
    },
    titleChange() {
      this.db("articles")
        .update({
          title: this.article.title,
          updated_at: new Date()
        })
        .where("id", this.article.id)
        .then();
      this.bus.$emit("changeTitle", this.article.title);
    }
  }
};
</script>
<style lang="scss" scoped>
#article {
  overflow: hidden;
  margin: 8px;
  margin-top: 0px;
  box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
  height: calc(100% - 8px);
  border-radius: 6px;
  display: flex;
  flex-flow: column;
}

.barBtn {
  width: 38px;
  height: 36px;
  line-height: 36px;
  text-align: center;
  cursor: pointer;
  color: #888;
  font-size: 14px;
}

.barBtn:hover {
  background: #e7f3ff;
  color: #007acc;
}
</style>
<style>
.modelWidth{
  width: 930px !important;
}
</style>
