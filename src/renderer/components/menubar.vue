<template>
    <div class="container">
        <div @click="goto('/articles')" class="item selected">
            知识
        </div>
        <div @click="goto('/flowers')" class="item">
            火花
        </div>
        <div @click="goto('/setting')" class="item">
            设置
        </div>
    </div>
</template>
<script>
    export default {
        methods: {
            goto(url) {
                this.$router.push(url);
            }
        }
    }
</script>
<style scoped>
    .container {
        width: 28px;
    }

    .item {
        width: 18px;
        padding-left: 3px;
        padding-right: 5px;
        word-break: normal;
        text-align: center;
        margin-bottom: 10px;
        background: #fff;
        padding-top: 8px;
        padding-bottom: 8px;
        color: #666;
        border-top-right-radius: 6px;
        border-bottom-right-radius: 6px;
        cursor: pointer;
        box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
    }

    .item:hover {
        box-shadow: 1px 1px 4px rgba(26, 26, 26, 0.3);
    }
</style>