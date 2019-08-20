<template>
  <div class="header">
    <div class="logo">
      <div>
        <img src="/logo.png" />
      </div>
      <div class="logoTitle">想学吗</div>
    </div>
    <div class="menu">
      <div>功能介绍</div>
      <div>免责声明</div>
      <div>二开说明</div>
      <div>联系方式</div>
    </div>
  </div>
</template>
<script>
export default {
    //滚动条滚动，下方出现border或阴影
}
</script>
<style lang="scss" scoped>
.header {
  height: 44px;
  line-height: 44px;
  position: fixed;
  z-index: 999999;
  display: flex;
  padding-left: 16%;
  padding-right: 16%;
  left: 0px;
  right: 0px;
  top: 0px;
  padding-top: 12px;
  padding-bottom: 12px;
  line-height: 44px;
  background: #f3f4f6;
  .logo {
    width: 260px;
    cursor: pointer;
  }
  .logoTitle {
    color: #666;
    font-size: 18px;
    font-weight: bolder;
    line-height: 44px;
  }
  .logo div {
    float: left;
  }
  .logo div img {
    height: 36px;
    margin-top: 4px;
    margin-right: 8px;
  }
  .menu {
    flex: 1;
  }
  .menu div {
    float: right;
    padding-left: 16px;
    padding-right: 16px;
    color: #555;
  }
  .menu div:hover {
    color: #0084ff;
    cursor: pointer;
  }
}
</style>