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
                    <div class="tag" v-for="(item,index) in searchTags">
                        <div class="tagText">{{item.title}}</div>
                        <div @click.stop="closeTag(index)" class="tagClose">
                            <i class="iconfont icon-guanbi" style="font-size: 14px !important;"></i>
                        </div>
                    </div>
                </div>
                <div :class="searchFocus?'searchContainerFocus':'searchContainer'">
                    <div class="searchInput" style="background: transparent;">
                        <input autocomplete="off" @keyup="search" v-model="searchText" placeholder="请输入搜索内容" @focus="searchFocus = true"
                            @blur="searchFocus = false" class="textInput" type="text" />
                    </div>
                    <div @click="search" class="searchBtn">
                        <i class="iconfont icon-search"></i>
                    </div>
                </div>
                <div style="flex: 1;"></div>
                <div @click="newArticleBtnClick" class="btn">添加知识</div>
            </div>
            <div class="articles box">
                <div class="noDataTip" v-if="articles.length<1">
                    <div>
                        向我自己的知识海洋进发
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
            search() {
                if (this.searchText.length > 36) {
                    swal({
                        icon: "error",
                        text: "您输入的内容太长了",
                    });
                    return;
                }
                var titleSearchArr = this.searchText.replace(/\s+/gi, '^').split('^');
                let rootQuery = this.db
                    .distinct()
                    .select("articles.*")
                    .from("articles")
                    .leftJoin("article_tag", "articles.id", "article_tag.article_id")
                this.searchTags.forEach(v => {
                    rootQuery.andWhere("article_tag.tag_id", v.id);
                })
                titleSearchArr.forEach((v, index) => {
                    if (index == 0) {
                        rootQuery.andWhere("articles.title", "like", "%" + v + "%");
                    } else {
                        rootQuery.orWhere("articles.title", "like", "%" + v + "%");
                    }
                });
                rootQuery.orderBy("articles.updated_at", "desc");
                rootQuery.then(rows => {
                    this.articles = rows;
                }).catch(function (e) {
                    console.log(e);
                });
            },
            newArticleBtnClick() {
                let article = {
                    title: '',
                };
                this.db("articles").insert(article).then(rows => {
                    article.id = rows[0];
                    let aPath = path.join(electron.remote.app.getPath('userData'), "/xxm/" + article.id);
                    fs.mkdirSync(aPath);
                    fs.writeFileSync(path.join(aPath, "/a.data"), "", {
                        encoding: 'utf8'
                    });
                    this.bus.$emit('findOrAddTab', {
                        url: '/article/' + article.id,
                        title: "",
                    });
                    this.bus.$emit('articleCount');
                })

            }
        },
        mounted: function () {
            this.search();
            this.bus.$on('removeTag', tagId => {
                let index = this.searchTags.findIndex(v => v.id == tagId);
                if (index < 0) {
                    return;
                }
                this.searchTags.splice(index, 1);
                this.search();
            })
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

    .rightContainer {
        float: right;
        width: 238px;
        height: 100%;
        padding-bottom: 8px;
        margin-right: 8px;
        display: flex;
        flex-flow: column;
    }

    .leftContainer {
        margin-right: 246px;
        overflow: hidden;
        height: 100%;
        display: flex;
        flex-flow: column;
    }

    .tagIndex:hover {
        background: #dcedfe;
    }
</style>