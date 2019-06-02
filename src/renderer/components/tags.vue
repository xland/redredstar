<template>
    <div class="tagContainer box">
        <div :key="item.id" @click="tagClick(item)" class="tag tagIndex" v-for="(item,index) in $root.tags">
            <div class="tagText">{{item.title}}</div>
        </div>
        <div class="noDataTip" v-if="$root.tags.length<1" style="font-size: 22px;">
	    标签库空空如也...
        </div>
    </div>
</template>
<script>
    export default {
        data() {
            return {}
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
            }
        },
        mounted() {
            //todo 根据refer num 排序
            this.db("tags").orderBy("created_at", "desc").then(rows => {
                this.$root.tags = rows;
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