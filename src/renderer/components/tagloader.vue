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
            placeholder="请输入知识的标签，Enter键创建" @keyup="tagInput($event)" class="textInput" />
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
        computed: {
            article() {
                return this.$root.a[this.$root.aIndex];
            }
        },
        methods: {
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
                    if (this.article.tagIds.length >= 6) {
                        swal({
                            icon: "error",
                            text: "最多输入6个标签",
                        });
                        return;
                    }
                    var tag = this.$root.t.find(item => {
                        return item.text == text;
                    })
                    if (tag && this.article.tagIds.includes(tag.id)) {
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
                    var notHaveFlag = !this.article.tagIds.includes(tag.id);
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
                this.article.tagIds.push(tag.id);
                tag.articleIds.push(this.article.id);
                this.tagInputText = "";
                this.findTagResult = [];
            }
        }
    }
</script>
<style scoped lang="scss">
    .tagTipContainer .tag {
        padding-right: 6px;
    }
    .tagloader{
    flex: 1;
    padding-top: 1px;
}
.tagloader input{
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