import { createRouter, createWebHistory } from 'vue-router';
import HomeView from '../views/HomeView.vue';
import DashboardView from '../views/DashboardView.vue';
import LogQueryView from '../views/LogQueryView.vue';
import AlertManageView from '../views/AlertManageView.vue';

const routes = [
  { path: '/', component: HomeView },
  { path: '/dashboard', component: DashboardView },
  { path: '/log-query', component: LogQueryView },
  { path: '/alert-manage', component: AlertManageView }
];

const router = createRouter({
  history: createWebHistory(),
  routes
});

export default router;  // ✅ 导出 router 实例
