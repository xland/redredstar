<template>
    <div class="tagloader">
        <div v-show="findTagResult.length > 0" class="findTagResult" :style="'left:'+tagInputLeft">
            <div class="tagTipContainer">
                <div @click="tagTipClick(index)" class="tag tagIndex" v-for="(item,index) in findTagResult">
                    <div class="tagText">{{item.text}}</div>
                </div>
            </div>
            <div class="arrow-down">
            </div>
        </div>
        <input autocomplete="off" @blur="tagInputBlur" @focus="tagInputFocus" v-model="tagInputText" type="text" id="tagInputBox"
            placeholder="请输入知识的标签，Enter键保存" @keyup="tagInput($event)" class="textInput" />
    </div>
</template>
<script>
    import swal from 'sweetalert';
    export default {
        data() {
            return {
                findTagResult: [],
                tagInputText: '',
                tagInputLeft: '16px',
            };
        },
        methods: {
            tagInputBlur() {
                // var self = this;
                // setTimeout(function () {
                //     self.tagInputText = "";
                //     self.findTagResult = [];
                // }, 518);
            },
            tagInputFocus() {
                this.tagInputLeft = (document.getElementById("tagInputBox").offsetLeft + 8) + 'px';
            },
            tagInput(e) {
                if (e.keyCode == 13) {
                    var text = this.tagInputText.trim();
                    if (text.length < 1) {
                        swal({
                            icon: "error",
                            text: "输入的标签为空",
                        })
                        return;
                    }
                    if (text.getByteLength() > 12) {
                        swal({
                            icon: "error",
                            text: "标签太长了",
                        });
                        return;
                    }
                    if (this.$root.article.tagIds.length >= 6) {
                        swal({
                            icon: "error",
                            text: "最多输入6个标签",
                        });
                        return;
                    }
                    var tag = this.$root.t.find(item => {
                        return item.text == text;
                    })
                    if (tag && this.$root.article.tagIds.includes(tag.id)) {
                        swal({
                            icon: "error",
                            text: "该文章已经存在该标签",
                        });
                        return;
                    }
                    if (!tag) {
                        tag = {
                            id: new Date().getTime(),
                            text,
                            refer: 0,
                            articleIds: []
                        };
                        this.$root.t.push(tag);
                    }
                    this.addTagRefer(tag);
                } else {
                    this.findTagResult = [];
                    var text = this.tagInputText.trim();
                    if (text.length > 0) {
                        this.findTag(text);
                    }
                }
            },
            findTag(text) {
                var regExp = new RegExp('.*' + text + '.*', 'gi');
                var arr = this.$root.t.filter(tag => {
                    var matchFlag = regExp.test(tag.text);
                    var notHaveFlag = !this.$root.article.tagIds.includes(tag.id);
                    return matchFlag && notHaveFlag;
                })
                this.findTagResult = arr;
            },
            tagTipClick(index) {
                var tag = this.findTagResult[index];
                this.addTagRefer(tag);
            },
            addTagRefer(tag) {
                tag.refer += 1;
                this.$root.article.tagIds.push(tag.id);
                tag.articleIds.push(this.$root.article.id);
                this.tagInputText = "";
                this.findTagResult = [];
            }
        }
    }
</script>
<style scoped lang="scss">
    .tagTipContainer .tag{
        padding-right: 6px;
    }
</style>