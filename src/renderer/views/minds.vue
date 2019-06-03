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
                        <input autocomplete="off" @keyup.13="search(false)" v-model="searchText" placeholder="请输入搜索内容"
                            @focus="searchFocus = true" @blur="searchFocus = false" class="textInput" type="text" />
                    </div>
                    <div @click="search(false)" class="searchBtn">
                        <i class="iconfont icon-search"></i>
                    </div>
                </div>
                <div style="flex: 1;"></div>
                <div @click="newArticleBtnClick" class="btn">添加脑图</div>
            </div>
            <div class="indexListContainer box" style="padding-top: 8px;">
                <div class="noDataTip" v-if="minds.length<1">
                    <div>
                        脑图库空空如也
                    </div>
                    <div @click="newArticleBtnClick" class="btn center" style="width: 80px;">
                        添加脑图
                    </div>
                </div>
                <minditem :key="item.id" :item="item" :index="index" v-for="(item,index) in minds"></minditem>
            </div>
        </div>
    </div>
</template>
<script>
    const fs = require('fs');
    const path = require('path');
    const electron = require("electron")
    import minditem from "../components/minditem";
    import list from "./mixins/list";
    export default {
        mixins: [list],
        components: {
            minditem,
        },
        data() {
            return {
                minds: [],
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
                    this.minds = [];
                }
                let query = this.db('minds').limit(16)
                    .orderBy("updated_at", "desc")
                    .offset(this.minds.length);
                if (this.searchText.trim().length > 0) {
                    let titleSearchArr = this.searchText.trim().replace(/\s+/gi, '^').split('^');
                    titleSearchArr.forEach(v => {
                        query = query.andWhere("title", "like", '%' + v + '%');
                    });
                }
                if (this.searchTags.length > 0) {
                    let tagIds = this.searchTags.map(v => v.id);
                    this.db("mind_tag").whereIn("tag_id", tagIds).then(atRows => {
                        let articleIds = atRows.map(v => v.mind_id);
                        query = query.whereIn("id", articleIds).then(result => {
                            if (result.length < 1) return;
                            this.minds = this.minds.concat(result);
                        })
                    })
                } else {
                    query = query.then(result => {
                        if (result.length < 1) return;
                        this.minds = this.minds.concat(result);
                    })
                }
            },
            newArticleBtnClick() {
                let now = new Date();
                let mind = {
                    title: '',
                    created_at: now,
                    updated_at: now,
                    visited_at: now
                };
                this.db("minds").insert(mind).then(rows => {
                    mind.id = rows[0];
                    let aPath = path.join(electron.remote.app.getPath('userData'), "/xxm/m_" + mind.id);
                    fs.mkdirSync(aPath);
                    let initData = {
                        "root": {
                            "data": {
                                "id": "node_0",
                                "created": now.getTime(),
                                "text": "",
                                "x":0,
                                "y":0,
                                "w":72,
                                "h":30,
                                "isNew":true,
                                "isSelected":true,
                                "isRoot":true
                            },
                            "children": []
                        },
                        "template": "default",
                        "theme": "default",
                        "version": "1.4.43"
                    };
                    fs.writeFileSync(path.join(aPath, "/m.data"), JSON.stringify(initData), this.$root.rwOption);
                    this.$router.push('/mind/' + mind.id)
                    this.bus.$emit('mindCount');
                })
            }
        }
    }
</script>
<style scoped lang="scss">
</style>