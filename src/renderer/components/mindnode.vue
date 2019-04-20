<template>
    <g>
        <g>
            <rect width="100" height="30" x="50%" y="50%" fill="#ff0033"></rect>
            <text>{{node.data.text}}</text>
        </g>
        <g>
            <!-- line -->
        </g>
        <mindnode :key="item.data.id" :node="item" v-for="item in node.children"></mindnode>
    </g>
</template>
<script>
    import mindnode from "./mindnode"
    export default {
        name:"mindnode",
        components: {
            mindnode
        },
        props: ['node'],
        data() {
            return {

            }
        }
    }
</script>