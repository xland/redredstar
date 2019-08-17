<template>
  <div id="index" class="view">
    <div class="rightContainer">
      <tags></tags>
      <!-- todo -->
      <div v-if="false" class="box" style="flex: 1;margin-bottom: 8px;"></div>
    </div>
    <div class="leftContainer">
      <div class="box searchAddContainer">
        <div v-if="false" class="searchBtn" style="margin-left: 8px;">
          <i class="iconfont icon-redu" style="font-size: 18px !important;"></i>
        </div>
        <div v-if="false" class="searchBtn">
          <i class="iconfont icon-shijian" style="font-size: 18px !important;"></i>
        </div>
        <div class="searchTagContainer">
          <div :key="item.id" class="tag" v-for="(item,index) in searchTags">
            <div class="tagText">{{item.title}}</div>
            <div @click.stop="closeTag(index)" class="tagClose">
              <i class="iconfont icon-guanbi" style="font-size: 14px !important;"></i>
            </div>
          </div>
        </div>
        <div :class="searchFocus?'searchContainerFocus':'searchContainer'">
          <div class="searchInput" style="background: transparent;">
            <input
              autocomplete="off"
              @keyup.13="search(false)"
              v-model="searchText"
              placeholder="请输入搜索内容"
              @focus="searchFocus = true"
              @blur="searchFocus = false"
              class="textInput"
              type="text"
            />
          </div>
          <div @click="search(false)" class="searchBtn">
            <i class="iconfont icon-search"></i>
          </div>
        </div>
        <div style="flex: 1;"></div>
        <div class="btn addBtn">
          <div @click="newArticleBtnClick" class="addBtnText">添加知识 {{this.$root.editorType}}</div>
          <div
            @mouseleave="addBtnElesShow = false"
            @mouseenter="addBtnElesShow = true"
            class="addBtnDrop"
          >
            <i class="iconfont icon-xiala"></i>
          </div>
          <div
            @click="newArticleBtnClick(false)"
            @mouseenter="addBtnElesShow = true"
            @mouseleave="addBtnElesShow = false"
            v-show="addBtnElesShow"
            class="addBtnMenu"
          >添加知识 {{this.$root.editorType == 'html'?'markdown':'html'}}</div>
        </div>
      </div>
      <div class="indexListContainer box" style="padding-top: 8px;">
        <div class="noDataTip" v-if="articles.length<1">
          <div>知识库空空如也</div>
          <div @click="newArticleBtnClick" class="btn center" style="width: 80px;">添加知识</div>
        </div>
        <articleitem :key="item.id" :item="item" :index="index" v-for="(item,index) in articles"></articleitem>
      </div>
    </div>
  </div>
</template>
<script>
const fs = require("fs");
const path = require("path");
const electron = require("electron");
import articleitem from "../components/articleitem";
import list from "./mixins/list";
export default {
  mixins: [list],
  components: {
    articleitem
  },
  data() {
    return {
      articles: [],
      hoverIndex: -1,
      addBtnElesShow: false
    };
  },
  methods: {
    async search(isGetMore) {
      if (this.searchText.length > 36) {
        swal({
          icon: "error",
          text: "您输入的内容太长了"
        });
        return;
      }
      if (!isGetMore) {
        this.articles = [];
      }
      let query = this.db("articles")
        .limit(38)
        .orderBy("updated_at", "desc")
        .offset(this.articles.length);
      if (this.searchText.trim().length > 0) {
        let titleSearchArr = this.searchText
          .trim()
          .replace(/\s+/gi, "^")
          .split("^");
        titleSearchArr.forEach(v => {
          query = query.andWhere("title", "like", "%" + v + "%");
        });
      }
      let result = [];
      if (this.searchTags.length > 0) {
        let tagIds = this.searchTags.map(v => v.id);
        let atRows = await this.db("article_tag").whereIn("tag_id", tagIds);
        let articleIds = atRows.map(v => v.article_id);
        articleIds = Array.from(new Set(articleIds));
        result = await query.whereIn("id", articleIds);
      } else {
        result = await query.then();
      }
      if (result.length < 1) return;
      this.articles = this.articles.concat(result);
    },
    async newArticleBtnClick(flag = true) {
      let now = new Date();
      let editor_type = this.$root.editorType;
      if (!flag)
        editor_type = this.$root.editorType == "html" ? "markdown" : "html";
      let article = {
        title: "",
        created_at: now,
        updated_at: now,
        visited_at: now,
        editor_type
      };
      let [id] = await this.db("articles").insert(article);
      article.id = id;
      let aPath = path.join(
        electron.remote.app.getPath("userData"),
        "/xxm/" + article.id
      );
      fs.mkdirSync(aPath);
      fs.writeFileSync(path.join(aPath, "/a.data"), "", this.$root.rwOption);
      this.$router.push("/article/" + article.id);
      this.bus.$emit("articleCount");
    }
  }
};
</script>
<style scoped lang="scss">
#index {
  .addBtn {
    display: flex;
    border: 1px solid #137ae3;
    border-radius: 0px;
    .addBtnText {
      flex: 1;
      padding-right: 8px;
    }
    .addBtnDrop {
      width: 26px;
      text-align: center;
      margin-right: -8px;
      background: #dcedfe;
      color: #137ae3;
      i {
        font-size: 10px !important;
      }
    }
    .addBtnDrop:hover {
      background: #137ae3;
      color: #fff;
    }
    .addBtnMenu {
      position: absolute;
      top: 45px;
      right: 262px;
      z-index: 999;
      height: 32px;
      line-height: 32px;
      padding-left: 8px;
      padding-right: 8px;
      background: #fff;
      color: #137ae3;
      border: 1px solid #137ae3;
    }
    .addBtnMenu:hover {
      background: #dcedfe;
      color: #137ae3;
    }
  }
}
</style>