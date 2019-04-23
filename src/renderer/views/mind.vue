<template>
    <div id="mind" class="view" v-if="mind">
        <svg width="100%" height="100%" xmlns="http://www.w3.org/2000/svg" version="1.1"
            xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:svgjs="http://svgjs.com/svgjs">
            <defs></defs>
            <g>
                <!-- root -->
                <g>
                    <rect width="100" height="30" x="50%" y="50%" fill="#ff0033"></rect>
                    <text>{{mind.root.data.text}}</text>
                </g>
                <mindnode :node="item" :key="item.data.id" v-for="item in mind.root.children">
                </mindnode>
            </g>

        </svg>
    </div>
</template>
<script>
    import SVG from "svg.js"
    const fs = require('fs');
    const path = require('path');
    import mindnode from "../components/mindnode"
    const {
        remote
    } = require('electron');
    export default {
        components: {
            mindnode
        },
        data() {
            return {
                mind: null
            }
        },
        beforeRouteUpdate(to, from, next) {
            this.$refs.articleEditor.saveContent(() => {
                next();
            });
        },
        beforeRouteLeave(to, from, next) {
            this.$refs.articleEditor.saveContent(() => {
                next();
            });
        },
        mounted() {
        },
        methods: {

        }
    };
</script>
<style lang="scss" scoped>
    #mind {
        overflow: hidden;
        margin: 8px;
        margin-top: 0px;
        box-shadow: 0 1px 3px rgba(26, 26, 26, 0.2);
        height: calc(100% - 8px);
        border-radius: 3px;
        display: flex;
        flex-flow: column;
        background: #fff;
    }
</style>