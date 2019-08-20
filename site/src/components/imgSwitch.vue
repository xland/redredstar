<template>
  <div class="imgSwitch">
    <swiper :options="swiperOption">
      <swiper-slide>
        <div class="imgBox">
          <img src="@/assets/imgs/articles.png" />
        </div>
      </swiper-slide>
      <swiper-slide>
        <div class="imgBox">
          <img src="@/assets/imgs/html.png" />
        </div>
      </swiper-slide>
      <swiper-slide>
        <div class="imgBox">
          <img src="@/assets/imgs/sites.png" />
        </div>
      </swiper-slide>
      <swiper-slide>
        <div class="imgBox">
          <img src="@/assets/imgs/flowers.png" />
        </div>
      </swiper-slide>
      <swiper-slide>
        <div class="imgBox">
          <img src="@/assets/imgs/mind.png" />
        </div>
      </swiper-slide>
      <swiper-slide>
        <div class="imgBox">
          <img src="@/assets/imgs/setting.png" />
        </div>
      </swiper-slide>
      <swiper-slide>
        <div class="imgBox">
          <img src="@/assets/imgs/data.png" />
        </div>
      </swiper-slide>
      <div class="swiper-pagination" slot="pagination"></div>
    </swiper>
  </div>
</template>
<script>
import "swiper/dist/css/swiper.css";
import { swiper, swiperSlide } from "vue-awesome-swiper";
export default {
  components: {
    swiper,
    swiperSlide
  },
  data() {
    return {
      swiperOption: {
        effect: "cube",
        autoplay: true,
        grabCursor: true,
        cubeEffect: {
          shadow: true,
          slideShadows: true,
          shadowOffset: 20,
          shadowScale: 0.94
        },
        pagination: {
          el: ".swiper-pagination"
        }
      }
    };
  }
};
</script>
<style lang="scss" scoped>
.imgSwitch {
  margin-top: 32px;
  height: 680px;
  .imgBox {
    width: 910px;
    height: 620px;
    overflow: hidden;
    box-shadow: 0 10px 20px rgba(0,0,0,.2);
  }
  img {
    margin-left: -5px;
    width: 920px;
    height: 625px;
  }
  .swiper-inner {
    position: relative;
    overflow: hidden;
    height: 600px;
    padding: 15px;
  }
  .swiper-container {
    width: 910px !important;
    height: 620px;
    position: absolute;
    left: 50%;
    top: 430px;
    margin-left: -455px;
  }
  .swiper-slide {
    background-position: center;
    background-size: cover;
  }
}
</style>