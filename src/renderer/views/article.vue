<template>
  <div id="article" class="view" v-if="article">
    <div class="blankLine">
      <div style="flex: 1;display: flex">
        <input autocomplete="off" id="articleTitleInput" @keydown.tab="titleTab" @input="titleChange" class="textInput"
          v-model="article.title" placeholder="请输入文章标题">
      </div>
      <div class="publishBtn" @click="showSites = true">
        <i class="iconfont icon-fabu" style="font-size: 18px !important;"></i>
      </div>
    </div>
    <editor ref="articleEditor"></editor>
    <articletag ref="articleTag"></articletag>
    <site v-if="showSites"></site>
  </div>
</template>
<script>
  import articletag from "../components/articletag";
  import site from "../components/site";
  import editor from "../components/editor";
  const ipcRenderer = require('electron').ipcRenderer;
  export default {
    components: {
      site,
      articletag,
      editor,
    },
    data() {
      return {
        showSites: false,
        article: null,
      };
    },
    beforeRouteUpdate(to, from, next) {
      this.showSites = false;
      this.getArticle(to.params.id);
      this.$refs.articleEditor.saveContent(() => {
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
        };
      },
      getArticle(id) {
        this.db("articles").where("id", id).select("*").then(rows => {
          this.article = rows[0];
          this.$nextTick(() => {
            this.$refs.articleTag.getTags();
            this.$refs.articleEditor.getContent();
            this.hookArticleRefresh();
            window.document.getElementById("articleTitleInput").focus();
          })
        })
      },
      titleTab() {
        setTimeout(function () {
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

  .publishBtn {
    width: 36px;
    height: 36px;
    line-height: 36px;
    text-align: center;
    background: #e7f3ff;
    cursor: pointer;
    color: #888;
    font-size: 14px;
  }

  .publishBtn:hover {
    background: #dcedfe;
    color: #007acc;
  }
</style>