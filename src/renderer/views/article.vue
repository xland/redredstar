<template>
  <div id="article" class="view" v-if="article">
    <div class="blankLine" style="border-top: 1px solid #e5e5e5;overflow: hidden;">
      <div style="flex: 1;display: flex;overflow: hidden;">
        <input autocomplete="off" id="articleTitleInput" @keydown.tab="titleTab" @input="titleChange" class="textInput"
          v-model="article.title" placeholder="请输入文章标题">
      </div>
      <div class="barBtn" @mouseenter="showRecentArticle = true" @mouseleave="showRecentArticle = false">
        <i class="iconfont icon-zuijin" style="font-size: 18px !important;"></i>
      </div>
      <div class="barBtn" @click="showSitesClick">
        <i class="iconfont icon-fabu" style="font-size: 18px !important;"></i>
      </div>
    </div>
    <editor ref="articleEditor"></editor>
    <articletag ref="articleTag"></articletag>
    <site v-show="showSites"></site>
    <div v-show="showRecentArticle" class="recentArticle" @mouseenter="showRecentArticle = true"
      @mouseleave="showRecentArticle = false">
      <div @click="$router.push('/article/' + item.id)" v-for="item in recentArticles" class="item">{{item.title}}</div>
    </div>
  </div>
</template>
<script>
  import articletag from "../components/articletag";
  import site from "../components/site";
  import editor from "../components/editor";
  const fs = require('fs');
  const path = require('path');
  const {
    remote
  } = require('electron');
  export default {
    components: {
      site,
      articletag,
      editor
    },
    data() {
      return {
        showSites: false,
        article: null,
        showRecentArticle: false,
        recentArticles: [],
      };
    },
    beforeRouteUpdate(to, from, next) {
      this.showSites = false;
      this.$refs.articleEditor.saveContent(() => {
        this.$refs.articleEditor.destroy();
        this.getArticle(to.params.id);
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
      showSitesClick() {
        this.showSites = true;
        if (this.article.editor_type == "html") {
          this.$refs.articleEditor.downloadInternetImg();
        }
      },
      hookArticleRefresh() {
        this.$root.articlePublushCb = (obj) => {
          this.db('article_site')
            .where("article_id", this.article.id)
            .andWhere("site_id", obj.siteId)
            .select("*").then(rows => {
              let asObj = {
                article_id: this.article.id,
                site_id: obj.siteId,
                edit_url: obj.url
              }
              if (rows.length < 1) {
                this.db("article_site").insert(asObj).then();
              } else {
                this.db("article_site").update(asObj).where("id", rows[0].id).then();
              }
            });
          this.$refs.articleEditor.jnaPublish()
        };
      },
      getArticle(id) {
        this.db("articles").where("id", id).select("*").then(rows => {
          this.article = rows[0];
          this.article.visited_at = new Date();
          this.db('articles').where("id", this.article.id).update(this.article).then(() => {
            this.db("articles").orderBy("visited_at", "desc").limit(8).offset(1).then(recentRows => {
              this.recentArticles = recentRows;
            })
          });
          this.$nextTick(() => {
            this.$refs.articleTag.getTags();
            this.$refs.articleEditor.getContent();
            this.hookArticleRefresh();
            window.document.getElementById("articleTitleInput").focus();
          })
        })
      },
      titleTab() {
        setTimeout(() => {
          this.$refs.articleEditor.focus();
        }, 80);
      },
      titleChange() {
        this.db("articles").update({
          title: this.article.title,
          updated_at: new Date()
        }).where("id", this.article.id).then();
        this.bus.$emit('changeTitle', this.article.title);
      }
    }
  };
</script>
<style lang="scss" scoped>
  .recentArticle {
    position: absolute;
    border-top: 1px solid #e5e5e5;
    line-height: 32px;
    right: 8px;
    top: 45px;
    background: #fff;
    box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
    border-bottom-left-radius: 6px;
    border-bottom-right-radius: 6px;
    padding-top: 6px;
    padding-bottom: 6px;
    z-index: 9;
  }

  .recentArticle .item {
    padding-left: 8px;
    padding-right: 8px;
    max-width: 256px;
    text-overflow: ellipsis;
    white-space: nowrap;
    overflow: hidden;
    color: #666;
    cursor: pointer;
  }

  .recentArticle .item:hover {
    background: #dcedfe;
    color: #007acc;
  }

  #article {
    overflow: hidden;
    margin: 8px;
    margin-top: 0px;
    box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
    height: calc(100% - 8px);
    border-radius: 3px;
    display: flex;
    flex-flow: column;
  }

  .barBtn {
    border-left: 1px solid #e5e5e5;
    width: 36px;
    height: 36px;
    line-height: 36px;
    text-align: center;
    background: #e7f3ff;
    cursor: pointer;
    color: #888;
    font-size: 14px;
  }

  .barBtn:hover {
    background: #dcedfe;
    color: #007acc;
  }
</style>