<template>
    <div class="tagContainer box">
        <div @click="tagClick(item)" class="tag tagIndex" v-for="(item,index) in tags">
            <div class="tagText">{{item.title}}</div>
        </div>
        <div class="noDataTip" v-if="tags.length<1" style="font-size: 22px;">
            还没有知识标签
        </div>
    </div>
</template>
<script>
    export default {
        data() {
            return {
                tags: []
            }
        },
        methods: {
            tagClick(tag) {
                var index = this.$parent.searchTags.findIndex(v => {
                    return v.id == tag.id
                });
                if (index >= 0) {
                    this.$parent.searchTags.splice(index, 1);
                    this.$parent.search();
                    return;
                }
                if (this.$parent.searchTags.length >= 3) {
                    swal({
                        icon: "error",
                        text: "最多同时检索三个标签",
                    });
                    return;
                }
                this.$parent.searchTags.push(tag);
                this.$parent.search();
            },
        },
        mounted() {
            this.db("tags").select("*").then(rows => {
                this.tags = rows;
            })
        }
    }
</script>
<style scoped>
    .tagContainer {
        overflow-y: auto;
        margin-top: 0px;
        margin-bottom: 8px;
        padding-right: 8px;
        flex: 1;
        padding-top: 11px;
        padding-bottom: 6px;
    }

    .tagIndex {
        padding-right: 6px;
    }
</style>