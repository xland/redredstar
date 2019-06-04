<template>
    <div class="item">
        <div v-show="$parent.editingIndex != index" @click="editClick(index)" class="content"
            v-html="item.content?item.content.replace(/\n/gi,'<br />'):'[内容为空]'">
        </div>
        <div v-if="$parent.editingIndex == index">
            <textarea placeholder="请输入您的思想火花" @keydown="saveBlur" @blur="saveBlur(true)" class="textInput ta"
                v-model="item.content"></textarea>
        </div>
        <div class="bottomRow">
            <flowertag :tags="item.tags"></flowertag>
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
    import flowertag from "./flowertag";
    export default {
        components:{flowertag},
        props: ['item', 'index'],
        data() {
            return {
            }
        },
        methods: {
            alert(str) {
                swal({
                    icon: "error",
                    text: str,
                })
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
            saveBlur(blur) {
                let ctrlAndS = event.keyCode == 83 && (event.metaKey || event.ctrlKey);
                if (ctrlAndS || blur == true) {
                    this.db("flowers")
                        .update({
                            content: this.item.content,
                            updated_at: new Date()
                        }).where("id", this.item.id)
                        .then(() => this.$parent.editingIndex = -1);
                }
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
                    var flower = this.$parent.flowers.splice(this.index, 1)[0];
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
        height: 22px;
    }

    .content {
        max-height: 80px;
        overflow: hidden;
        text-overflow: ellipsis;
        line-height: 26px;
        font-size: 13px;
        color: #333;
        white-space: space break-all;
    }

    .timeBox {
        width: 168px;
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