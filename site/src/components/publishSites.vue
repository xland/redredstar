<template>
  <div id="platform" class="platform">
    <div class="title">“想学吗”应用内的文章支持一键发布到如下平台</div>
    <div class="siteBox">
      <div class="siteItem" :key="item.code" v-for="item in mediaSites">
        <div>
          <img :src="`/logos/${item.code}/logo.png`" alt />
        </div>
        <div class="name">{{item.name}}</div>
      </div>
    </div>
    <div class="siteBox">
      <div class="siteItem" :key="item.code" v-for="item in blogSites">
        <div>
          <img :src="`/logos/${item.code}/logo.png`" alt />
        </div>
        <div class="name">{{item.name}}</div>
      </div>
    </div>
  </div>
</template>
<style lang="scss" scoped>
.platform {
  padding-top: 60px;
  padding-bottom: 60px;
  .title {
    height: 38px;
    line-height: 38px;
    padding-bottom: 20px;
    padding-top: 20px;
    font-size: 22px;
    color: #717682;
    font-weight: bold;
  }
  .siteBox {
    display: flex;
    padding-left: 16%;
    padding-right: 16%;
    justify-content: center;
    padding-top: 38px;
    color: #717682;
    .siteItem {
      padding-left: 28px;
      padding-right: 28px;
      img {
        width: 38px;
        height: 38px;
      }
      .name {
        height: 46px;
        line-height: 46px;
        font-size: 16px;
      }
    }
  }
}
</style>
<script>
export default {
  data() {
    return {
      mediaSites: [
        {
          code: "baijiahao",
          name: "百家号"
        },
        {
          code: "dayuhao",
          name: "百家号"
        },
        {
          code: "iqiyi",
          name: "爱奇艺"
        },
        {
          code: "qiehao",
          name: "企鹅号"
        },
        {
          code: "souhuhao",
          name: "搜狐号"
        },
        {
          code: "weixin",
          name: "公众号"
        },
        {
          code: "yidianhao",
          name: "一点号"
        }
      ],
      blogSites: [
        {
          code: "cnblogs",
          name: "博客园"
        },
        {
          code: "csdn",
          name: "CSDN"
        },
        {
          code: "douban",
          name: "豆瓣"
        },
        {
          code: "oschina",
          name: "开源中国"
        },
        {
          code: "zhihu",
          name: "知乎"
        },
        {
          code: "jianshu",
          name: "简书"
        }
      ]
    };
  }
};
</script>