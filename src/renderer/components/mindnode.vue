<template>
    <g :id="node.data.id" class="node" :transform="`translate(${leftValue},${node.data.y})`">
        <g @click="nodeSelect">
            <rect :class="[node.data.isSelected?'rectSelected':'']" width="100" height="30"></rect>
            <text stroke-width="0" fill="#fff" transform="translate(24,20)"
                :id="node.data.id+'_t'">{{node.data.text||'[未命名]'}}</text>
            <!-- line -->
        </g>
        <mindnode :key="item.data.id" :node="item" :isAtRight="item.data.x > 0" v-for="item in node.children">
        </mindnode>
    </g>
</template>
<script>
    import mindnode from "./mindnode"
    export default {
        name: "mindnode",
        components: {
            mindnode
        },
        props: ['node', 'isAtRight'],
        data() {
            return {

            }
        },
        computed: {
            leftValue() {
                if (this.node.data.x > 0 && !this.isAtRight) {
                    return 0 - this.node.data.x;
                }
                return this.node.data.x;
            }
        },
        methods: {
            nodeSelect() {
                this.node.data.isSelected = !this.node.data.isSelected;
                if (this.node.data.isSelected) {
                    this.bus.$emit('resetSelectNode', this.node);
                } else {
                    this.bus.$emit('resetSelectNode', null);
                }
            }
        }
    }
</script>