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
                        <div class="tagText">{{item.content}}</div>
                        <div @click.stop="closeTag(index)" class="tagClose">
                            <i class="iconfont icon-guanbi" style="font-size: 14px !important;"></i>
                        </div>
                    </div>
                </div>
                <div :class="searchFocus?'searchContainerFocus':'searchContainer'">
                    <div class="searchInput" style="background: transparent;">
                        <input autocomplete="off" @keyup="search" v-model="searchText" placeholder="请输入搜索内容"
                            @focus="searchFocus = true" @blur="searchFocus = false" class="textInput" type="text" />
                    </div>
                    <div @click="search" class="searchBtn">
                        <i class="iconfont icon-search"></i>
                    </div>
                </div>
                <div style="flex: 1;"></div>
                <div @click="showAddBox = true" class="btn">添加思想火花</div>
                <div v-if="showAddBox" @click='showAddBox = false' class="maskExceptMenu" style="overflow: hidden;">
                    <div @click.stop class="addBoxInnerDiv">
                        <textarea v-model="content" placeholder="记录我的思想火花..." class="textInput ta"></textarea>
                        <div @click="saveFlower" class="btn saveBtn">
                            提交
                        </div>
                    </div>
                    <div style="position: absolute;right: -30px;bottom: -30px;transform:rotate(45deg);">
                        <i style="font-size: 220px;color: #dadada;" class="iconfont icon-yanhua"></i>
                    </div>
                </div>
            </div>
            <div class="articles box">
                <div class="noDataTip" v-if="flowers.length<1">
                    <div>
                        思想火花空空如也...
                    </div>
                    <div @click="showAddBox = true" class="btn center" style="width: 80px;">
                        添加思想火花
                    </div>
                </div>
                <floweritem :key="item.id" :item="item" :index="index" v-for="(item,index) in flowers">
                    {{item.content}}
                </floweritem>
            </div>
        </div>
    </div>
</template>
<script>
    const fs = require('fs');
    const path = require('path');
    const electron = require("electron")
    import floweritem from "../components/floweritem";
    import tags from "../components/tags";
    export default {
        components: {
            floweritem,
            tags
        },
        data() {
            return {
                searchFocus: false,
                flowers: [],
                searchTags: [],
                searchText: '',
                showAddBox: false,
                content: "",
                hoverIndex: -1,
            }
        },
        methods: {
            saveFlower() {
                let now = new Date();
                let flower = {
                    content: this.content.replace(/\n/g,"<br/>"),
                    updated_at: now,
                    created_at: now
                };
                this.db("flowers").insert(flower).then(() => {
                    this.initData(false);
                    this.showAddBox = false;
                });

            },
            closeTag(index) {
                this.searchTags.splice(index, 1);
                this.search();
            },
            search() {
                return;
                if (this.searchText.length > 36) {
                    swal({
                        icon: "error",
                        text: "您输入的内容太长了",
                    });
                    return;
                }
                let titleSearchArr = this.searchText.replace(/\s+/gi, '^').split('^');
                let result = this.allArticles.filter(v => titleSearchArr.some(str => v.title.includes(str)));
                result = result.filter(v => this.searchTags.every(st => v.tagIds.includes(st.id)));
                this.articles = result;
            },
            newFlowerBtnClick() {},
            initData(needSearch) {
                //todo:不要一下子都搞出来
                this.db("flowers").orderBy("updated_at", "desc").then(rows => {
                    this.flowers = rows.map(v => {
                        if (!v.content) {
                            v.content = "";
                        }
                        v.tagIds = [];
                        return v;
                    })
                    if (needSearch) {
                        this.search();
                    }
                    this.db("flower_tag").select("*").then(fts => {
                        fts.forEach(f_t => {
                            let flower = this.flowers.find(a => a.id == a_t.flower_id);
                            flower.tagIds.push(a_t.tag_id);
                        })
                    })
                });
            }
        },
        mounted: function () {
            this.initData(false);
            this.bus.$on('removeTag', tagId => {
                let index = this.searchTags.findIndex(v => v.id == tagId);
                if (index < 0) {
                    return;
                }
                this.searchTags.splice(index, 1);
                this.search();
            });
            this.bus.$on('articleFromWebApp', () => {
                this.initData(true);
            })
        }
    }
</script>
<style scoped lang="scss">
    .addBoxInnerDiv {
        width: 360px;
        margin-left: auto;
        box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
        margin-top: 60px;
        margin-right: auto;
        background: #fff;
        border-radius: 6px;
    }

    .saveBtn {
        float: right;
        margin-bottom: 8px;
        width: 58px;
        text-align: center;
    }

    .ta {
        height: 82px;
        padding-top: 8px;
        padding-bottom: 8px;
        line-height: 26px;
        width: calc(100% - 16px);
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