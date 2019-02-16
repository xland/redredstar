<template>
    <div id="index">
        <div class="rightContainer">
            <div class="tagContainer box">
                <div :title="item.text+'['+item.refer+']'" @click="tagClick(item)" class="tag tagIndex" v-for="(item,index) in $root.t">
                    <div class="tagText">{{item.text}}</div>
                </div>
                <div class="noDataTip" v-if="$root.t.length<1" style="font-size: 22px;">
                    还没有知识标签
                </div>
            </div>
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
                        <div class="tagText">{{item.text}}</div>
                        <div @click.stop="closeTag(index)" class="tagClose">
                            <i class="iconfont icon-guanbi" style="font-size: 10px !important;"></i>
                        </div>
                    </div>
                </div>
                <div :class="searchFocus?'searchContainerFocus':'searchContainer'">
                    <div class="searchInput" style="background: transparent;">
                        <input autocomplete="off" @keyup.enter="search" v-model="searchText" placeholder="请输入搜索内容"
                            @focus="searchFocus = true" @blur="searchFocus = false" class="textInput" type="text" />
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
    var fs = require('fs');
    var path = require('path');
    import articleitem from "../components/articleitem";
    export default {
        components: {
            articleitem
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
                var self = this;

                self.articles = this.$root.a.filter(item => {
                    var result = self.searchTags.every(searchTag => {
                        return item.tagIds.some(id => {
                            return id == searchTag.id
                        })
                    });
                    if (!result) {
                        return false;
                    }
                    result = titleSearchArr.some(subStr => {
                        //搜索文本如果是空格隔开的关键字，搜索时用或关系；
                        return item.title.includes(subStr);
                    });
                    return result;
                }).sort((i1, i2) => {
                    return i2.update - i1.update
                });
            },
            tagClick(tag) {
                var index = this.searchTags.findIndex(v => {
                    return v.id == tag.id
                });
                if (index >= 0) {
                    this.searchTags.splice(index, 1);
                    return;
                }
                if (this.searchTags.length >= 3) {
                    swal({
                        icon: "error",
                        text: "最多同时检索三个标签",
                    });
                    return;
                }
                this.searchTags.push(tag);
                this.search();
            },
            newArticleBtnClick() {
                var article = {
                    id: new Date().getTime(),
                    title: '',
                    views: 1,
                    tagIds: []
                };
                fs.mkdirSync(path.join(this.$root.basePath, article.id.toString()));
                fs.writeFileSync(path.join(this.$root.basePath, article.id + "/a.data"), '');
                this.$root.a.push(article);
                this.bus.$emit('addTab', {
                    url: '/article/' + article.id,
                    text: "[未命名]",
                });
            }
        },
        mounted: function () {
            this.search();
        }
    }
</script>
<style scoped lang="scss">
    #index {
        height: 100%;
    }

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

    .noDataTip {
        margin-top: 12%;
        text-align: center;
        font-size: 32px;
        line-height: 66px;
        color: #bbb;
    }

    .tagContainer {
        overflow-y: auto;
        margin-top: 0px;
        margin-bottom: 8px;
        padding-right: 8px;
        flex: 1;
        padding-top: 11px;
        padding-bottom: 6px;
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

    .tagIndex {
        padding-right: 6px;
    }

    .tagIndex:hover {
        background: #dcedfe;
    }
</style>