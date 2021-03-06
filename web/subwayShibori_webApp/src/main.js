import Vue from 'vue'
import VueRouter from 'vue-router'
import App from './App.vue'
import Home from './Home.vue'
import ColorPicker from './ColorPicker.vue'

Vue.use(VueRouter);

export const eventBus = new Vue();

const routes = [{
    path: '/',
    component: Home
  },
  {
    path: '/color_customization',
    component: ColorPicker
  }
];

const router = new VueRouter({
  routes: routes,
  mode: 'history'
})

new Vue({
  el: '#app',
  router: router,
  render: h => h(App)
})
