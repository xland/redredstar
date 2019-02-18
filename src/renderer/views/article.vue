<template>
  <div id="article" class="view" v-if="$root.aIndex>=0">
    <div class="blankLine">
      <div style="flex: 1;display: flex">
        <input autocomplete="off" id="articleTitleInput" @keydown.meta.83="savekeyUp" @keydown.tab="titleTab" @change="titleChange"
          class="textInput" v-model="article.title" placeholder="请输入文章标题">
      </div>
      <div class="publishBtn" @click="publishBtnClick">
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
    var fs = require('fs');
    var path = require('path');
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
    computed:{
      article(){
        return this.$root.a[this.$root.aIndex];
      }
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
      self.$root.save(function () {
        next(); //跳转到其他页面前，要先把当前的文章保存一下；
        self.$root.aIndex = -1;
      });
    },
    mounted() {
      this.getArticle(this.$route.params.id);
    },
    methods: {
      publishBtnClick(){
        this.$root.save();
        this.showSites = true;
      },
      getTags() {
        var arr = this.$root.t.filter(v => {
          return this.article.tagIds.some(tagId => {
            return tagId == v.id;
          })
        })
        return arr;
      },
      getArticle(id) {
        this.$root.aIndex = this.$root.a.findIndex(item => {
          return item.id == id;
        });
        var self = this;
        this.$nextTick(function () { 
          window.document.getElementById("articleTitleInput").focus();
          window.editorContentReady(self.article.id);
        })
      },
      titleTab() {
        setTimeout(function () {
          document.getElementById("ueditor_0").contentWindow.document.body.focus();
        }, 80);
      },
      savekeyUp(e) {
        self.$root.saveUAT("a");
      },
      titleChange() {
        this.$root.u.tabs[this.$root.u.tabIndex].text = this.article.title;
      },
      removeTag(index) {
        var tagId = this.article.tagIds.splice(index, 1)[0];
        var parentIndex = this.$root.t.findIndex(v => {
          return v.id == tagId;
        });
        var tag = this.$root.t[index];
        if (tag.refer <= 1) {
          this.$root.t.splice(parentIndex, 1);
        } else {
          var articleIdIndex = tag.articleIds.indexOf(this.article.id);
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