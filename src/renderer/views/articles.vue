<template>
    <div id="index" class="view">
        <div class="rightContainer">
            <tags></tags>
            <!-- todo -->
            <div v-if="false" class="box" style="flex: 1;margin-bottom: 8px;">
            </div>
        </div>
        <div class="leftContainer">
            <div class="box searchAddContainer">
                <div v-if="false" class="searchBtn" style="margin-left: 8px;">
                    <i class="iconfont icon-redu" style="font-size: 18px !important;"></i>
                </div>
                <div v-if="false" class="searchBtn">
                    <i class="iconfont icon-shijian" style="font-size: 18px !important;"></i>
                </div>
                <div style="max-width: 386px;overflow-x: auto;padding-top: 4px;">
                    <div :key="item.id" class="tag" v-for="(item,index) in searchTags">
                        <div class="tagText">{{item.title}}</div>
                        <div @click.stop="closeTag(index)" class="tagClose">
                            <i class="iconfont icon-guanbi" style="font-size: 14px !important;"></i>
                        </div>
                    </div>
                </div>
                <div :class="searchFocus?'searchContainerFocus':'searchContainer'">
                    <div class="searchInput" style="background: transparent;">
                        <input autocomplete="off" @keyup="search(false)" v-model="searchText" placeholder="请输入搜索内容"
                            @focus="searchFocus = true" @blur="searchFocus = false" class="textInput" type="text" />
                    </div>
                    <div @click="search(false)" class="searchBtn">
                        <i class="iconfont icon-search"></i>
                    </div>
                </div>
                <div style="flex: 1;"></div>
                <div @click="newArticleBtnClick" class="btn">添加知识</div>
            </div>
            <div class="indexListContainer box">
                <div class="noDataTip" v-if="articles.length<1">
                    <div>
                        知识库空空如也
                    </div>
                    <div @click="newArticleBtnClick" class="btn center" style="width: 80px;">
                        添加知识
                    </div>
                </div>
                <articleitem :key="item.id" :item="item" :index="index" v-for="(item,index) in articles"></articleitem>
            </div>
        </div>
    </div>
</template>
<script>
    const fs = require('fs');
    const path = require('path');
    const electron = require("electron")
    import articleitem from "../components/articleitem";
    import list from "./mixins/list";
    export default {
        mixins: [list],
        components: {
            articleitem,
        },
        data() {
            return {
                articles: [],
                hoverIndex: -1,
            }
        },
        methods: {
            search(isGetMore) {
                if (this.searchText.length > 36) {
                    swal({
                        icon: "error",
                        text: "您输入的内容太长了",
                    });
                    return;
                }
                if (!isGetMore) {
                    this.articles = [];
                }
                let query = this.db('articles').limit(16)
                    .orderBy("updated_at", "desc")
                    .offset(this.articles.length);
                if (this.searchText.trim().length > 0) {
                    let titleSearchArr = this.searchText.trim().replace(/\s+/gi, '^').split('^');
                    titleSearchArr.forEach(v => {
                        query = query.andWhere("title", "like", '%' + v + '%');
                    });
                }
                if (this.searchTags.length > 0) {
                    let tagIds = this.searchTags.map(v => v.id);
                    this.db("article_tag").whereIn("tag_id", tagIds).then(atRows => {
                        let articleIds = atRows.map(v => v.article_id);
                        articleIds = Array.from(new Set(articleIds));
                        query = query.whereIn("id", articleIds).then(result => {
                            if (result.length < 1) return;
                            this.articles = this.articles.concat(result);
                        })
                    })
                } else {
                    query = query.then(result => {
                        if (result.length < 1) return;
                        this.articles = this.articles.concat(result);
                    })
                }
            },
            newArticleBtnClick() {
                let now = new Date();
                let article = {
                    title: '',
                    created_at: now,
                    updated_at: now,
                    visited_at: now,
                    editor_type: this.$root.editorType,
                };
                this.db("articles").insert(article).then(rows => {
                    article.id = rows[0];
                    let aPath = path.join(electron.remote.app.getPath('userData'), "/xxm/" + article.id);
                    fs.mkdirSync(aPath);
                    fs.writeFileSync(path.join(aPath, "/a.data"), "", this.$root.rwOption);
                    this.$router.push('/article/' + article.id)
                    this.bus.$emit('articleCount');
                })
            }
        }
    }
</script>
<style scoped lang="scss">
</style>