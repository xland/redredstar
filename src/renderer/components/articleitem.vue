<template>
    <div @mouseout="$parent.hoverIndex = -1" @mouseover="$parent.hoverIndex = index" @click="articleClick()" class="item">
        <div @click.stop="delArticle(index)" class="delBox" v-show="$parent.hoverIndex == index">
            <i class="iconfont icon-shanchu"></i>
        </div>
        <div class="timeBox" v-show="$parent.hoverIndex != index">
            {{timeFormat(item.update)}}
        </div>
        <div style="margin-right: 80px">
            {{item.title?item.title:"[未命名]"}}
        </div>
    </div>
</template>
<script>
    var fs = require('fs');
    var path = require('path');
    import swal from 'sweetalert';
    export default {
        props: ['item', 'index'],
        data() {
            return {

            }
        },
        methods: {
            articleClick() {
                this.bus.$emit('findOrAddTab', {
                    url: '/article/' + this.item.id,
                    text: this.item.title ? this.item.title : "[未命名]",
                });
            },
            timeFormat(time) {
                return window.getSimpleTime(parseInt(time));
            },
            delArticle() {
                var self = this;
                swal({
                    icon: "warning",
                    text: "确实要删除此项知识吗？",
                    buttons: [
                        "取消", "删除"
                    ]
                }).then((value) => {
                    if (!value) return;
                    //删界面
                    var article = self.$parent.articles.splice(self.index, 1)[0];
                    //删标签库
                    article.tagIds.forEach(id => {
                        var parentIndex = self.$root.t.findIndex(tag => {
                            return tag.id == id
                        });
                        var tag = this.$root.t[parentIndex];
                        if (tag.refer <= 1) {
                            this.$root.t.splice(parentIndex, 1);
                        } else {
                            var articleIdIndex = tag.articleIds.indexOf(article.id);
                            if (articleIdIndex >= 0) {
                                tag.articleIds.splice(articleIdIndex, 1);
                            }
                            tag.refer -= 1;
                        }
                    });
                    //删文章库
                    var articleIndex = self.$root.a.findIndex(v => {
                        return v.id == article.id;
                    });
                    self.$root.a.splice(articleIndex, 1);
                    //删TAB页
                    self.bus.$emit('removeTab', {
                        url: '/article/' + article.id
                    })
                    //删文件
                    var dir = path.join(self.$root.basePath, article.id.toString());
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