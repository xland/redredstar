<template>
    <g :id="node.data.id" class="node" :transform="`translate(${node.data.x},${node.data.y})`">
        <g @click="nodeSelect">
            <rect :class="[node.data.isSelected?'rectSelected':'']" width="100" height="30"></rect>
            <text stroke-width="0" fill="#fff" transform="translate(24,20)"
                :id="node.data.id+'_t'">{{node.data.text||'[未命名]'}}</text>
            <!-- line -->
        </g>
        <mindnode :key="item.data.id" :node="item" v-for="item in node.children"></mindnode>
    </g>
</template>
<script>
    import mindnode from "./mindnode"
    export default {
        name: "mindnode",
        components: {
            mindnode
        },
        props: ['node'],
        data() {
            return {

            }
        },
        methods: {
            nodeSelect() {
                this.node.data.isSelected = !this.node.data.isSelected;
                if(this.$parent.selectedNode){
                    this.$parent.selectedNode.data.isSelected = false;
                }
                this.$parent.selectedNode = this.node;
            }
        }
    }
</script>