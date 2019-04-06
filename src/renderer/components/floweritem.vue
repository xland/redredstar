<template>
    <div @mouseout="$parent.hoverIndex = -1" @mouseover="$parent.hoverIndex = index" @click="articleClick()"
        class="item">
        <div @click.stop="delArticle(index)" class="delBox" v-show="$parent.hoverIndex == index">
            <i class="iconfont icon-shanchu"></i>
        </div>
        <div class="timeBox" v-show="$parent.hoverIndex != index">
            {{item.updated_at | getSimpleTime}}
        </div>
        <div style="margin-right: 80px" v-html="item.content?item.content:'[未命名]'">
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
            articleClick() {
                this.$router.push('/article/' + this.item.id)
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
    .timeBox {
        float: right;
        width: 68px;
        color: #999;
        font-size: 12px;
        text-align: right;
        padding-right: 6px;
    }

    .delBox {
        float: right;
        width: 38px;
        font-size: 12px;
        text-align: center;
        color: #F1403C;
    }

    .delBox:hover {
        background: #F1403C;
        color: #fff;
    }

    .item {
        overflow: hidden;
        height: 38px;
        line-height: 38px;
        font-size: 13px;
        cursor: pointer;
        margin-left: 8px;
        margin-right: 8px;
        padding-left: 6px;
        text-overflow: ellipsis;
        border-bottom: 1px solid #f6f6f6;
    }

    .item:hover {
        background: #f6f6f6;
    }
</style>