<template>
  <g :id="node.data.id" :class="classValue" :transform="`translate(${node.data.x},${node.data.y})`">
    <g class="gChild">
      <node :key="item.data.id" :prop-data="item" v-for="item in node.children"></node>
    </g>
    <g class="gRec">
      <rect @click.stop="nodeClick" :width="node.data.w" :height="node.data.h"></rect>
      <text @click.stop="nodeClick" transform="translate(12,20)">{{node.data.text||'[未命名]'}}</text>
      <foreignObject
        @mousedown.stop
        @keydown.stop
        v-if="isEdit"
        x="0"
        y="0"
        :width="node.data.w"
        :height="node.data.h"
      >
        <input v-model="editTxt" class="svgInput" type="text">
      </foreignObject>
      <path :d="pathValue"></path>
      <use
        @click="plus"
        v-if="node.data.isClose"
        y="10"
        :x="node.data.x > 0?-9:node.data.w"
        xlink:href="#plus"
      ></use>
      <use
        @click="subtract"
        v-if="node.children.length > 1 && !node.data.isClose"
        y="10"
        :x="node.data.x > 0?-9:node.data.w"
        xlink:href="#subtract"
      ></use>
    </g>
  </g>
</template>
<script>
import node from "./node";
import common from "./common";
import { setTimeout } from "timers";
export default {
  mixins: [common],
  name: "node",
  components: {
    node
  },
  props: ["propData"],
  data() {
    return {
      isSelected: false,
      node: this.propData
    };
  },
  mounted() {
    let self = this;
    this.bus.$on("addSubNode", () => {
      if (!self.isSelected) return;
      let x = self.node.data.x > 0 ? self.node.data.w + 128 : 0 - 72 - 128;
      self.addSubNode(x);
    });
    this.bus.$on("delNode", () => {
      if (!self.isSelected) return;
      self.delNode();
    });
  },
  computed: {
    pathValue() {
      let startX = 0;
      let startY = 15;
      let endX = 0 - this.node.data.x + this.$parent.node.data.w;
      let endY = 0 - this.node.data.y + 15;
      let span = -10;
      if(this.node.data.x < 0){
        startX = this.node.data.w;
        endX = 0 - this.node.data.x;
        span = 10;
      }
      return `M${startX},${startY}L${startX+span},${startY}C${startX+6*span},${startY},${endX-6*span},${endY},${endX},${endY}`;
    },
    classValue() {
      let arr = new Set(["node"]);
      this.node.data.id.split("_").length == 3
        ? arr.add("nodeGrade2")
        : arr.delete("nodeGradeElese");
      this.isSelected ? arr.add("nodeSelected") : arr.delete("nodeSelected");
      return [...arr];
    }
  },
  methods: {
    delNode() {
      let index = this.$parent.node.children.findIndex(
        m => m.data.id == this.node.data.id
      );
      let node = this.$parent.node.children.splice(index, 1)[0];
      this.$nextTick(() => {
        this.$parent.reLocation(node.data.x > 0);
      });
    },
    subtract() {
      document.querySelector(`#${this.node.data.id} > .gChild`).style.display =
        "none"; //why can not set isClose directly
      this.$parent.reLocation(this.node.data.x > 0);
      this.node.data.isClose = true;
      this.bus.$emit("needSave");
    },
    plus() {
      document.querySelector(`#${this.node.data.id} > .gChild`).style.display =
        "inherit";
      this.reLocation(this.node.data.x > 0);
      this.node.data.isClose = false;
      this.bus.$emit("needSave");
    }
  }
};
</script>
<style scoped>
.gRec path {
  fill: none;
  stroke: #0084ff;
}
</style>
