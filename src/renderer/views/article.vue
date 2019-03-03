<template>
  <div id="article" class="view" v-if="article">
    <div class="blankLine">
      <div style="flex: 1;display: flex">
        <input autocomplete="off" id="articleTitleInput" @keydown.tab="titleTab" @input="titleChange" class="textInput"
          v-model="article.title" placeholder="请输入文章标题">
      </div>
      <div class="publishBtn" @click="publishBtnClick">
        <i class="iconfont icon-fabu" style="font-size: 18px !important;"></i>
      </div>
    </div>
    <div style="flex: 1;background: #fff;">
    </div>
    <articletag ref="articleTag"></articletag>
    <site v-if="showSites"></site>
  </div>
</template>
<script>
  import articletag from "../components/articletag";
  import site from "../components/site";
  export default {
    components: {
      site,
      articletag
    },
    data() {
      return {
        showSites: false,
        article: null,
      };
    },

    beforeRouteUpdate(to, from, next) {
      this.getArticle(to.params.id);
      next();
    },
    // beforeRouteLeave(to, from, next) {
    //   //跳转到其他页面前，要先把当前的文章保存一下；
    //   // this.$root.save();
    //   // next();
    //   // this.$root.aIndex = -1;
    // },
    mounted() {
      let articleId = this.$route.params.id;
      this.getArticle(articleId);
    },
    methods: {
      publishBtnClick() {
        this.$root.save();
        this.showSites = true;
      },
      getArticle(id) {
        this.db("articles").where("id", id).select("*").then(rows => {
          this.article = rows[0];
          var self = this;
          this.$nextTick(function () {
            self.$refs.articleTag.getTags(id);
            window.document.getElementById("articleTitleInput").focus();
          })
        })
      },
      titleTab() {
        setTimeout(function () {
          document.getElementById("ueditor_0").contentWindow.document.body.focus();
        }, 80);
      },
      titleChange() {
        this.db("articles").update({
          title: this.article.title
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