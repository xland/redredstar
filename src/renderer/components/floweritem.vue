<template>
    <div @mouseleave="$parent.hoverIndex = -1" @mouseenter="$parent.hoverIndex = index" @click="flowerClick()" class="item">
        <div class="content" v-html="item.content?item.content:'[未命名]'">
        </div>
        <div class="bottomRow">
            <div style="flex: 1;">
                <div class="rowTag">123</div>
                <div class="rowTag">标签标签</div>
                <div class="rowTag">123</div>
            </div>
            <div class="timeBox" v-show="$parent.hoverIndex != index">
                {{item.updated_at | getSimpleTime}}
            </div>
            <div @click.stop="delArticle(index)" class="delBox" v-show="$parent.hoverIndex == index">
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

            }
        },
        methods: {
            flowerClick() {
                
            },
            delArticle() {
                swal({
                    icon: "warning",
                    text: "确实要删除此项知识吗？",
                    buttons: [
                        "取消", "删除"
                    ]
                }).then((value) => {
                    if (!value) return;
                    //删界面
                    var article = this.$parent.articles.splice(this.index, 1)[0];
                    //删标签库
                    this.db("article_tag")
                        .where("article_id", article.id)
                        .select("*").then(at_rows => {
                            at_rows.forEach(v => {
                                this.db("article_tag")
                                    .count('id as count')
                                    .where("tag_id", v.tag_id).then(tc_rows => {
                                        if (tc_rows[0].count <= 1) {
                                            this.bus.$emit('removeTag', v.tag_id);
                                        }
                                        this.db("article_tag").where({
                                            id: v.id
                                        }).del().then();
                                    })
                            })
                        })
                    //删文章库
                    this.db("articles").where("id", article.id).del().then();
                    //删文件
                    let basePath = path.join(electron.remote.app.getPath('userData'), "/xxm");
                    var dir = path.join(basePath, article.id.toString());
                    var files = fs.readdirSync(dir);
                    files.forEach(function (file, index) {
                        fs.unlinkSync(path.join(dir, file));
                    });
                    fs.rmdirSync(dir);
                });
            },
        }
    }
</script>
<style scoped lang="scss">
    .bottomRow {
        display: flex;
        margin-top: 8px;
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
    }

    .timeBox {
        width: 68px;
        color: #999;
        font-size: 12px;
        text-align: right;
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