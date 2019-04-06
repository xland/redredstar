<template>
    <div class="blankLine">
        <div :key="item.id" class="tag" v-for="(item,index) in tags">
            <div class="tagText">{{item.title}}</div>
            <div @click.stop="removeTag(index)" class="tagClose">
                <i class="iconfont icon-guanbi" style="font-size: 10px !important;"></i>
            </div>
        </div>
        <div class="tagloader">
            <div v-show="findTagResult.length > 0" class="findTagResult" :style="'left:'+tagInputLeft">
                <div class="tagTipContainer">
                    <div @click="addTagFinish(item)" :key="item.id" class="tag tagIndex" v-for="(item,index) in findTagResult">
                        <div class="tagText">{{item.title}}</div>
                    </div>
                </div>
                <div class="arrow-down">
                </div>
            </div>
            <input autocomplete="off" @blur="tagInputBlur" @focus="tagInputFocus" v-model="tagInputText" type="text" id="tagInputBox"
                placeholder="请输入知识的标签，Enter键创建" @keyup="tagInput($event)" class="textInput" />
        </div>
    </div>
</template>
<script>
    import swal from 'sweetalert';
    export default {
        data() {
            return {
                id: -1,
                tags: [],
                findTagResult: [],
                tagInputText: '',
                tagInputLeft: '16px',
            };
        },
        methods: {
            alert(str) {
                swal({
                    icon: "error",
                    text: str,
                })
            },
            removeTag(index) {
                let item = this.tags.splice(index, 1)[0];
                this.db("article_tag")
                    .where("tag_id", item.id)
                    .andWhere("article_id", this.$parent.article.id)
                    .del()
                    .then(() => {
                        this.db("article_tag")
                            .count('id as count')
                            .where("tag_id", item.id)
                            .then(rows => {
                                if (rows[0].count < 1) {
                                    this.db("tags")
                                        .where({
                                            id: item.id
                                        })
                                        .del().then();
                                }
                            });
                    });
            },
            getTags() {
                this.db
                    .distinct()
                    .select("tags.*")
                    .from("tags")
                    .leftJoin("article_tag", "tags.id", "article_tag.tag_id")
                    .where("article_tag.article_id", this.$parent.article.id).then(rows => {
                        this.tags = rows;
                    })
            },
            tagInputBlur() {
                var self = this;
                setTimeout(function () {
                    self.tagInputText = "";
                    self.findTagResult = [];
                }, 518);
            },
            tagInputFocus() {
                this.tagInputLeft = (document.getElementById("tagInputBox").offsetLeft + 8) + 'px';
            },
            tagInput(e) {
                if (e.keyCode == 13) {
                    var text = this.tagInputText.trim();
                    if (text.length < 1) {
                        this.alert("输入的标签为空");
                        return;
                    }
                    if (text.getByteLength() > 12) {
                        this.alert("标签太长了");
                        return;
                    }
                    if (this.tags.length >= 6) {
                        this.alert("最多输入6个标签");
                        return;
                    }
                    let hasIt = this.tags.some(item => {
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
                } else {
                    this.findTag();
                }
            },
            addTagFinish(tag) {
                this.db("article_tag").insert({
                    article_id: this.$parent.article.id,
                    tag_id: tag.id
                }).then();
                this.db("articles")
                    .update({
                        "updated_at": new Date()
                    })
                    .where("id", this.$parent.article.id)
                    .then();
                this.tags.push(tag);
                this.tagInputText = "";
                this.findTagResult = [];
                this.$nextTick(() => {
                    this.tagInputFocus();
                })
            },
            findTag() {
                this.findTagResult = [];
                var text = this.tagInputText.trim();
                if (text.length < 1) {
                    return;
                }
                let query = this.db("tags")
                    .where("title", "like", '%' + text + '%')
                    .select("*")
                    .then(rows => {
                        rows.forEach(v => {
                            if (!this.tags.some(item => item.id == v.id)) {
                                this.findTagResult.push(v);
                            }
                        })
                    });
            },
        }
    }
</script>
<style scoped lang="scss">
    .tagTipContainer .tag {
        padding-right: 6px;
    }

    .tagloader {
        flex: 1;
        padding-top: 1px;
    }

    .tagloader input {
        width: 100%
    }

    .tagTipContainer {
        box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
        height: 36px;
        background: #fff;
        line-height: 36px;
        background: #f6f6f6;
        border-radius: 3px;
        display: flex;
        justify-content: center;
        align-items: center;
        padding-right: 8px;
    }

    .findTagResult {
        position: absolute;
        bottom: 72px;
        z-index: 99;
    }

    .arrow-down {
        width: 0;
        height: 0;
        border-left: 6px solid transparent;
        border-right: 6px solid transparent;
        border-top: 6px solid #f6f6f6;
    }
</style>