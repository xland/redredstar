<template>
    <div class="tagRowContainer">
        <div @mouseleave="hoverIndex = -1" @mouseenter="hoverIndex = index" class="rowTag" :key="iTag.id"
            v-for="(iTag,index) in tags">
            <div class="rowTagText">
                {{iTag.title}}
            </div>
            <div @click="del(index)" v-show="hoverIndex == index" class="rowTagBtn">
                <i class="iconfont icon-guanbi" style="font-size: 14px !important;"></i>
            </div>
        </div>
        <div class="rowTag tagInputContainer" v-show="showInput">
            <input v-model="tagInputText" @blur="showInput = false" @keyup.13="addTag()" placeholder="Enter键保存"
                class="textInput tagInput" />
        </div>
        <div v-show="!showInput" @click="showTagInput()" class="rowTag tagAddBtn">+</div>
    </div>
</template>
<script>
    export default {
        props: ['tags'],
        data() {
            return {
                showInput: false,
                tagInputText: "",
                hoverIndex: -1,
            }
        },
        methods: {
            del(index) {
                let item = this.tags.splice(index, 1)[0];
                this.db("flower_tag")
                    .where("tag_id", item.id)
                    .andWhere("flower_id", this.$parent.item.id)
                    .del()
                    .then(() => {
                        this.$root.delNoReferTag(item.id)
                    });
            },
            alert(str) {
                swal({
                    icon: "error",
                    text: str,
                })
            },
            showTagInput() {
                this.showInput = true;
                this.$nextTick(() => {
                    document.querySelector(".tagInput").focus();
                })
            },
            addTag() {
                var text = this.tagInputText.trim();
                if (text.length < 1) {
                    this.alert("输入的标签为空");
                    return;
                }
                if (this.tags.length >= 6) {
                    this.alert("最多输入6个标签");
                    return;
                }
                let hasIt = this.tags.some(item => item.title == text);
                if (hasIt) {
                    this.alert("该火花已经存在该标签");
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
                                this.$root.tags.unshift(tag);
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
                    flower_id: this.$parent.item.id,
                    tag_id: tag.id
                }).then();
                this.db("flowers").update({
                    "updated_at": new Date()
                }).where("id", this.$parent.item.id).then();
                this.tags.push(tag);
                this.tagInputText = "";
            },
        }
    }
</script>
<style scoped>
    .tagAddBtn {
        font-size: 14px !important;
        width: 10px;
        text-align: center;
    }

    .tagRowContainer {
        flex: 1;
        display: flex;
    }

    .tagInputContainer {
        box-shadow: inset 0px 0px 0px 1px #007acc !important;
    }

    .tagInput {
        padding-left: 2px;
        padding-right: 2px;
        width: 66px;
        font-size: 12px;
    }

    .rowTag {
        background: #e7f3ff;
        color: #888;
        display: inline-block;
        padding-left: 6px;
        padding-right: 6px;
        border-radius: 3px;
        font-size: 12px;
        margin-right: 5px;
        display: flex;
        text-align: center;
    }

    .rowTagText {
        max-width: 72px;
        overflow: hidden;
        white-space: nowrap;
        text-overflow: ellipsis;
        overflow: hidden;
        flex: 1;
    }

    .rowTagBtn {
        width: 10px;
    }

    .rowTagBtn:hover {
        color: red;
    }

    .rowTag:hover {
        color: #007acc;
    }
</style>