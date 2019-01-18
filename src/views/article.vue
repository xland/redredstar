<template>
  <div id="article" v-if="$root.article">
    <div class="blankLine">
      <div style="flex: 1;display: flex">
        <input autocomplete="off" id="articleTitleInput" @keydown.meta.83="savekeyUp" @keydown.tab="titleTab" @change="titleChange"
          class="textInput" v-model="$root.article.title" placeholder="请输入文章标题">
      </div>
      <div class="publishBtn" @click="showSites = true">
        <i class="iconfont icon-fabu" style="font-size: 18px !important;"></i>
      </div>
    </div>
    <div style="flex: 1;background: #fff;">
    </div>
    <div class="blankLine">
      <div class="tag" v-for="(item,index) in getTags()">
        <div class="tagText">{{item.text}}</div>
        <div @click.stop="removeTag(index)" class="tagClose">
          <i class="iconfont icon-guanbi" style="font-size: 10px !important;"></i>
        </div>
      </div>
      <tagloader></tagloader>
    </div>
    <site v-if="showSites"></site>
  </div>
</template>
<script>
  const fs = nw.require('fs');
  const path = nw.require('path');
  import tagloader from "../components/tagloader";
  import site from "../components/site";
  export default {
    components: {
      site,
      tagloader
    },
    data() {
      return {
        showSites: false,
      };
    },
    beforeRouteUpdate(to, from, next) {
      var self = this;
      self.$root.save(function () { //两篇文章切换，也要先保存一下上一篇文章；
        self.getArticle(to.params.id);
        next();
      });
    },
    beforeRouteLeave(to, from, next) {
      var self = this;
      self.$root.showEditor = false;
      self.$root.save(function () {
        next(); //跳转到其他页面前，要先把当前的文章保存一下；
      });
    },
    watch:{
      "$root.article": {
        handler: function (val, oldVal) {
          if(this.$root.article){
            this.$root.article.update = new Date().getTime();
          }
        },
        deep: true
      },
    },
    mounted() {
      this.getArticle(this.$route.params.id);
    },
    methods: {
      getTags() {
        var arr = this.$root.t.filter(v => {
          return this.$root.article.tagIds.some(tagId => {
            return tagId == v.id;
          })
        })
        return arr;
      },
      getArticle(id) {
        var doc = this.$root.a.find(item => {
          return item.id == id;
        })
        this.$root.article = doc;
        this.$root.showEditor = true;
        this.$nextTick(function () {
          window.document.getElementById("articleTitleInput").focus();
          window.editorContentReady(doc.id);
        })
      },
      titleTab() {
        setTimeout(function () {
          document.getElementById("ueditor_0").contentWindow.document.body.focus();
        }, 80);
      },
      savekeyUp(e) {
        if(this.$root.needSave.a){
          this.$root.save();
        }
      },
      titleChange() {
        var title = this.$root.article.title;
        this.$root.u.tabs[this.$root.u.tabIndex].text = title;
      },
      removeTag(index) {
        var tagId = this.$root.article.tagIds.splice(index, 1)[0];
        var parentIndex = this.$root.t.findIndex(v => {
          return v.id == tagId;
        });
        var tag = this.$root.t[index];
        if (tag.refer <= 1) {
          this.$root.t.splice(parentIndex, 1);
        } else {
          var articleIdIndex = tag.articleIds.indexOf(this.$root.article.id);
          if (articleIdIndex >= 0) {
            tag.articleIds.splice(articleIdIndex, 1);
          }
          tag.refer -= 1;
        }
      },
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