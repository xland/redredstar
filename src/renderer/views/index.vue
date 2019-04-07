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
                        <input autocomplete="off" @keyup.13="search(false)" v-model="searchText" placeholder="请输入搜索内容"
                            @focus="searchFocus = true" @blur="searchFocus = false" class="textInput" type="text" />
                    </div>
                    <div @click="search(false)" class="searchBtn">
                        <i class="iconfont icon-search"></i>
                    </div>
                </div>
                <div style="flex: 1;"></div>
                <div @click="newArticleBtnClick" class="btn">添加知识</div>
            </div>
            <div class="articles box">
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
    import tags from "../components/tags";
    export default {
        components: {
            articleitem,
            tags
        },
        data() {
            return {
                searchFocus: false,
                articles: [],
                searchTags: [],
                searchText: '',
                hoverIndex: -1,
            }
        },
        methods: {
            closeTag(index) {
                this.searchTags.splice(index, 1);
                this.search();
            },
            search(isGetMore) {
                if (this.searchText.length > 36) {
                    swal({
                        icon: "error",
                        text: "您输入的内容太长了",
                    });
                    return;
                }
                if(!isGetMore){
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
            },
            handleScroll() {
                var dom = document.querySelector(".articles");
                if (dom.scrollHeight - dom.scrollTop - dom.offsetHeight < 2) {
                    this.search(true);
                }
            },
            removeSearchTag(tagId) {
                let index = this.searchTags.findIndex(v => v.id == tagId);
                if (index < 0) {
                    return;
                }
                this.searchTags.splice(index, 1);
                this.search();
            }
        },
        beforeDestroy() {
            document.querySelector(".articles").removeEventListener('scroll', this.handleScroll);
        },
        mounted: function () {
            document.querySelector(".articles").addEventListener('scroll', this.handleScroll);
            this.search();
            this.bus.$on('removeTag', tagId => this.removeSearchTag(tagId));
            this.bus.$on('articleFromWebApp', this.search)
        }
    }
</script>
<style scoped lang="scss">
    .searchBtn {
        width: 38px;
        line-height: 28px;
        text-align: center;
        color: #1787fb;
        cursor: pointer;
    }

    .searchBtn:hover {
        background: #fff;
        color: #137ae3;
    }

    #index .addBtn {
        height: 28px;
        background: #1787fb;
        color: #fff;
        width: 80px;
        border-radius: 3px;
        text-align: center;
        line-height: 28px;
        margin-right: 12px;
        cursor: pointer;
    }

    #index .addBtn:hover {
        background: #137ae3;
    }

    .articles::-webkit-scrollbar {
        width: 2px;
        background-color: #f6f6f6;
    }

    .articles::-webkit-scrollbar-track {
        background: #fff;
    }

    .articles::-webkit-scrollbar-thumb {
        border-radius: 2px;
        background-color: #bbb;
    }

    .articles {
        flex: 1;
        margin-left: 8px;
        margin-right: 8px;
        overflow-y: hidden;
        overflow-x: hidden;
        margin-bottom: 8px;
        padding-bottom: 8px;
        padding-top: 8px;

    }

    .articles:hover {
        overflow-y: auto;
    }

    .searchContainer {
        background: #f6f6f6;
        width: 188px;
        border-radius: 3px;
        overflow: hidden;
        border: 1px solid #eee;
        margin-right: 18px;
        display: flex;
        margin-left: 12px;
        height: 28px;
    }

    .searchContainerFocus {
        background: #fff;
        border: 1px solid #e7f3ff;
        width: 188px;
        border-radius: 3px;
        overflow: hidden;
        margin-right: 18px;
        display: flex;
        height: 28px;
        margin-left: 12px;
    }

    .searchInput {
        margin-right: -28px;
        line-height: 28px;
        display: block;
        flex: 1;
        background: transparent;
    }

    .searchAddContainer {
        height: 46px;
        margin-left: 8px;
        margin-right: 8px;
        margin-bottom: 8px;
        display: flex;
        align-items: center;
    }

    .tagIndex:hover {
        background: #dcedfe;
    }
</style>