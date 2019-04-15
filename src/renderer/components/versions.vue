<template>
    <div class="container">
        <div class="block" v-for="item in versions">
            <div class="title">
                <div class="dot"></div>
                <div class="titleText">
                    V {{item.v}}
                </div>
            </div>
            <div v-html="item.i.replace(/\n/gi,'<br />')" class="content">
            </div>
        </div>
    </div>
</template>
<script>
import versions from "../utils/version"
export default {
    data(){
        return {
            versions
        }
    }
}
</script>
<style scoped>
    .container {
        margin-top: 16px;
        margin-left: 26px;
    }

    .block:last-child {
        border-left: none !important;
    }

    .block {
        border-left: 1px solid #1787fb;
    }

    .title {
        position: relative;
        left: -5px;
        top: -12px;
    }

    .dot {
        background: #1787fb;
        width: 10px;
        height: 10px;
        border-radius: 5px;
        display: inline-block;
    }

    .titleText {
        color: #1787fb;
        font-weight: bolder;
        display: inline-block;
        padding-left: 8px;
    }

    .content {
        margin-top: -8px;
        padding-left: 16px;
        padding-bottom: 18px;
        color: #555;
    }
</style>