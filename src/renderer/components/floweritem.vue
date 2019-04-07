<template>
    <div @click="flowerClick()" class="item">
        <div v-show="$parent.editingIndex != index" @click="editClick(index)" class="content"
            v-html="item.content?item.content:'[未命名]'">
        </div>
        <div v-if="$parent.editingIndex == index">
            <textarea @keydown="saveBlur(true)" @blur="saveBlur(false)" class="textInput ta"
                v-model="item.content"></textarea>
        </div>
        <div class="bottomRow">
            <div style="flex: 1;">
                <div class="rowTag">123</div>
                <div class="rowTag">标签标签</div>
                <div class="rowTag">123</div>
                <div class="rowTag" v-if="$parent.newTagIndex == index"
                    style="box-shadow:inset 0px 0px 0px 1px #007acc !important;">
                    <input v-model="tagInputText" 
                        @blur="$parent.newTagIndex = -1"
                        @keyup.13="addTag()" placeholder="Enter键保存"
                        class="textInput tagInput" />
                </div>
                <div v-if="$parent.newTagIndex != index" @click="showTagInput(index)" class="rowTag"
                    style="font-size: 14px;width: 10px;">+</div>
            </div>
            <div class="timeBox">
                {{item.updated_at | getSimpleTime}}
            </div>
            <div @click.stop="delArticle(index)" class="delBox">
                <i class="iconfont icon-shanchu"></i>
            </div>
        </div>
    </div>
</template>
<script>
    var fs = require('fs');
    var path = require('path');
    const electron = require('electron');
    import swal from 'sweetalert';
    export default {
        props: ['item', 'index'],
        data() {
            return {
                tagInputText: "",
            }
        },
        methods: {
            alert(str) {
                swal({
                    icon: "error",
                    text: str,
                })
            },
            addTag() {
                var text = this.tagInputText.trim();
                if (text.length < 1) {
                    this.alert("输入的标签为空");
                    return;
                }
                if (text.getByteLength() > 12) {
                    this.alert("标签太长了");
                    return;
                }
                if (this.item.tags.length >= 6) {
                    this.alert("最多输入6个标签");
                    return;
                }
                let hasIt = this.item.tags.some(item => {
                    return item.title == text;
                });
                if (hasIt) {
                    this.alert("该文章已经存在该标签");
                    return;
                }
                this.db("tags")
                    .where("title", text)
                    .select("*")
                    .then(rows => {
                        if (rows.length < 1) {
                            let tag = {
                                title: text
                            };
                            this.db("tags").insert(tag).then(rows => {
                                tag.id = rows[0];
                                this.addTagFinish(tag)
                            })
                            this.bus.$emit('tagCount');
                        } else {
                            this.addTagFinish(rows[0]);
                        }
                    })
            },
            addTagFinish(tag) {
                this.db("flower_tag").insert({
                    flower_id: this.item.id,
                    tag_id: tag.id
                }).then();
                this.db("flowers").update({
                    "updated_at": new Date()
                }).where("id", this.item.id).then();
                this.item.tags.push(tag);
                this.tagInputText = "";
            },
            showTagInput(index) {
                this.$parent.newTagIndex = index;
                this.$nextTick(() => {
                    document.querySelector(".tagInput").focus();
                })
            },
            editClick(index) {
                this.$parent.editingIndex = index
                this.$nextTick(() => {
                    document.querySelector("textarea").focus();
                })
            },
            saveBlur(needSave) {
                if (!needSave) {
                    this.$parent.editingIndex = -1;
                    return;
                }
                if (event.keyCode == 83 && (event.metaKey || event.ctrlKey)) {
                    this.db("flowers")
                        .update({
                            content: this.item.content
                        })
                        .where({
                            id: this.item.id
                        }).then(() => {
                            this.$parent.editingIndex = -1;
                        });
                }
            },
            flowerClick() {

            },
            delArticle() {
                swal({
                    icon: "warning",
                    text: "确实要删除此思维火花吗？",
                    buttons: [
                        "取消", "删除"
                    ]
                }).then((value) => {
                    if (!value) return;
                    //删界面
                    var flower = this.$parent.flowers.splice(this.index, 1)[0];
                    //删标签库
                    this.db("flower_tag")
                        .where("flower_id", flower.id)
                        .select("*").then(at_rows => {
                            at_rows.forEach(v => {
                                this.db("flower_tag").where("id", v.id).del().then(() => {
                                    this.$root.delNoReferTag(v.tag_id)
                                });
                            })
                        });
                    this.db("flowers").where("id", flower.id).del().then();
                });
            },
        }
    }
</script>
<style scoped lang="scss">
    .tagInput {
        padding-left: 2px;
        padding-right: 2px;
        width: 66px;
        font-size: 12px;
    }

    .ta {
        height: 60px;
        line-height: 26px;
        font-size: 13px;
        color: #333;
        border-radius: 3px;
        width: calc(100% - 18px);
        border: 1px solid #d7e3ef;
    }

    .bottomRow {
        display: flex;
        margin-top: 6px;
        line-height: 22px;
    }

    .rowTag {
        background: #e7f3ff;
        color: #888;
        cursor: pointer;
        display: inline-block;
        padding-left: 6px;
        padding-right: 6px;
        border-radius: 3px;
        font-size: 12px;
    }

    .rowTag:hover {
        color: #007acc;
    }

    .content {
        max-height: 80px;
        overflow: hidden;
        text-overflow: ellipsis;
        line-height: 26px;
        font-size: 13px;
        color: #333;
        white-space: nowrap;
    }

    .timeBox {
        width: 68px;
        color: #999;
        font-size: 12px;
        text-align: right;
        margin-right: 12px;
    }

    .delBox {
        width: 22px;
        height: 22px;
        line-height: 22px;
        overflow: hidden;
        font-size: 11px;
        text-align: center;
        color: #F1403C;
        border-radius: 3px;
    }

    .delBox:hover {
        background: #F1403C;
        color: #fff;
    }

    .item {
        padding-top: 8px;
        padding-bottom: 8px;
        cursor: pointer;
        margin-left: 8px;
        margin-right: 8px;
        border-bottom: 1px solid #f6f6f6;
    }
</style>