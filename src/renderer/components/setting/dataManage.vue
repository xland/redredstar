<template>
  <div id="dataManage">
    必读：
    <br />目前导入数据会覆盖原有数据（以后会考虑增加增量导入的功能）；
    <br />导入数据之前建议先导出数据以为备份；
    <br />导出的数据文件为zip格式，修改一下扩展名你就可以看里面的数据；
    <div>
      <div @click="importData" class="btn">导入数据</div>
      <div @click="exportData" class="btn export">导出数据</div>
    </div>
  </div>
</template>
<script>
const { remote } = require("electron");
const fs = require("fs-extra");
const path = require("path");
const adm_zip = require("adm-zip");
import swal from "sweetalert";
import { setTimeout } from "timers";
export default {
  data() {
    return {};
  },
  methods: {
    importData() {
      let fullFileName = remote.dialog.showOpenDialogSync({
        filters: [{ name: "备份文件", extensions: ["xxm", "zip"] }],
        properties: ["openFile"]
      })[0];
      if (!fullFileName) return;
      let srcPath = path.join(remote.app.getPath("userData"), "/xxm/");
      let destPath = path.join(remote.app.getPath("userData"), "/xxm_import/");
      fs.remove(destPath);
      var unzip = new adm_zip(fullFileName);
      unzip.extractAllTo(destPath, true);
      swal({ icon: "info", text: "数据导入成功,2秒后刷新数据" });
      setTimeout(function() {
        window.location.reload();
      }, 3000);
    },
    exportData() {
      let fullFileName = remote.dialog.showSaveDialogSync({
        title: "导出数据",
        defaultPath: "data.xxm"
      });
      let srcPath = path.join(remote.app.getPath("userData"), "/xxm/");
      if (!fullFileName) return;
      if (fs.existsSync(fullFileName)) {
        swal({ icon: "info", text: "文件已存在" });
        return;
      }
      var output = fs.createWriteStream(fullFileName, { flags: "wx" });
      var zip = new adm_zip();
      zip.addLocalFolder(srcPath);
      zip.writeZip(fullFileName);
      swal({ icon: "info", text: "数据导出成功" });
    }
  }
};
</script>
<style scoped lang="scss">
#dataManage {
  padding: 8px 18px 8px 18px;
  .btn {
    margin-top: 12px;
    display: inline-block;
    height: 38px;
    width: 130px;
    line-height: 38px;
    text-align: center;
  }
  .export {
    background: #666 !important;
  }
}
</style>


