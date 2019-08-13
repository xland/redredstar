<template>
  <div
    @mouseout="$parent.hoverIndex = -1"
    @mouseover="$parent.hoverIndex = index"
    @click="itemClick()"
    class="item"
  >
    <div @click.stop="del(index)" class="delBox" v-show="$parent.hoverIndex == index">
      <i class="iconfont icon-shanchu"></i>
    </div>
    <div class="timeBox" v-show="$parent.hoverIndex != index">{{item.updated_at | getSimpleTime}}</div>
    <div style="margin-right: 80px">{{item.title?item.title:"[未命名]"}}</div>
  </div>
</template>
<script>
var fs = require("fs");
var path = require("path");
const electron = require("electron");
import swal from "sweetalert";
export default {
  props: ["item", "index"],
  data() {
    return {};
  },
  methods: {
    itemClick() {
      this.$router.push("/mind/" + this.item.id);
    },
    async del() {
      let value = await swal({
        icon: "warning",
        text: "确实要删除此脑图吗？",
        buttons: ["取消", "删除"]
      });
      if (!value) return;
      //删界面
      var mind = this.$parent.minds.splice(this.index, 1)[0];
      //删标签库
      let at_rows = await this.db("mind_tag")
        .where("mind_id", mind.id)
        .select("*");
      at_rows.forEach(async v => {
        await this.db("mind_tag")
          .where("id", v.id)
          .del();
        await this.$root.delNoReferTag(v.tag_id);
      });
      //删文章库
      await this.db("minds")
        .where("id", mind.id)
        .del();
      //删文件
      let basePath = path.join(electron.remote.app.getPath("userData"), "/xxm");
      var dir = path.join(basePath, "m_" + mind.id.toString());
      var files = fs.readdirSync(dir);
      files.forEach(function(file, index) {
        fs.unlinkSync(path.join(dir, file));
      });
      fs.rmdirSync(dir);
    }
  }
};
</script>
<style scoped lang="scss">
.timeBox {
  float: right;
  width: 68px;
  color: #999;
  font-size: 12px;
  text-align: right;
  padding-right: 6px;
}

.delBox {
  float: right;
  width: 38px;
  font-size: 12px;
  text-align: center;
  color: #f1403c;
}

.delBox:hover {
  background: #f1403c;
  color: #fff;
}

.item {
  overflow: hidden;
  height: 38px;
  line-height: 38px;
  font-size: 13px;
  cursor: pointer;
  margin-left: 8px;
  margin-right: 8px;
  padding-left: 6px;
  text-overflow: ellipsis;
  border-bottom: 1px solid #f6f6f6;
}

.item:hover {
  background: #f6f6f6;
}
</style>