<template>
  <div>
    <div class="title">想学吗——个人知识管理与自媒体营销工具</div>
    <div class="subTitle">更快的、私有化的、标签式的、持续演进的个人知识管理与自媒体营销工具</div>
    <div class="download">
      <div @click="download('exe')" :class="{cur:osType == 'win'}">下载 Windows版</div>
      <div @click="download('dmg')" :class="{cur:osType == 'mac'}">下载 Mac版</div>
      <div @click="download('AppImage')" :class="{cur:osType == 'linux'}">下载 Linux版</div>
    </div>
    <div class="version">当前最新版本：6.4.8（注意所有操作系统只支持64位版本）</div>
  </div>
</template>
<script>
export default {
  data() {
    return {
      osType: "win"
    };
  },
  mounted() {
    this.getOsType();
  },
  methods: {
    getOsType() {
      if (navigator.userAgent.includes("Windows")) {
        this.osType = "win";
      } else if (navigator.userAgent.includes("Mac OS")) {
        this.osType = "mac";
      } else {
        this.osType = "linux";
      }
    },
    download(ext){
        window.location.href = `http://xiangxuema.com/download/latest.${ext}`
    }
  }
};
</script>
<style lang="scss" scoped>
.title {
  font-size: 36px;
  font-weight: bolder;
  color: #555;
  margin-top: 68px;
  padding-top: 80px;
}
.subTitle {
  padding-top: 36px;
  line-height: 28px;
  font-size: 18px;
  color: #666;
}
.download {
  margin-top: 38px;
}
.download div {
  display: inline-block;
  height: 56px;
  line-height: 56px;
  width: 160px;
  text-align: center;
  border-radius: 6px;
  background: #fff;
  margin-left: 16px;
  margin-right: 16px;
  border: 1px solid #ccc;
  cursor: pointer;
}
.download div:hover {
  background: #0064ef;
  color: #fff;
  border: 1px solid #0064ef;
}
.download .cur {
  background: #0084ff;
  color: #fff;
  border: 1px solid #0084ff;
}
.version {
  margin-top: 12px;
  line-height: 38px;
  font-size: 14px;
  color: #717682;
}
</style>