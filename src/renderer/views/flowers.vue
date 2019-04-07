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
            <div class="indexListContainer box">
                <div class="noDataTip" v-if="flowers.length<1">
                    <div>
                        思想火花空空如也...
                    </div>
                    <div @click="showAddBox = true" class="btn center" style="width: 80px;">
                        添加思想火花
                    </div>
                </div>
                <floweritem :key="item.id" :item="item" :index="index" v-for="(item,index) in flowers"></floweritem>
            </div>
        </div>
    </div>
</template>
<script>
    const fs = require('fs');
    const path = require('path');
    const electron = require("electron")
    import floweritem from "../components/floweritem";
    import list from "./mixins/list";
    export default {
        mixins: [list],
        components: {
            floweritem,
        },
        data() {
            return {
                flowers: [],
                showAddBox: false,
                content: "",
                editingIndex: -1,
                newTagIndex:-1,
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
        },
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
</style>