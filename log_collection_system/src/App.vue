<script setup>
import { useRouter } from 'vue-router';

const router = useRouter();
const navigate = (path) => {
  router.push(path);
};
</script>

<template>
  <div class="app-container">
    <!-- 顶部导航栏 -->
    <nav class="top-menu">
      <button @click="navigate('/')">首页</button>
      <button @click="navigate('/dashboard')">仪表盘</button>
      <button @click="navigate('/log-query')">日志查询</button>
      <button @click="navigate('/alert-manage')">告警管理</button>
    </nav>

    <div class="main-layout">
      <!-- 左侧菜单栏 -->
      <aside class="side-menu">
        <p>Kafka 信息</p>
        <p>Elasticsearch 信息</p>
      </aside>

      <!-- 页面内容 -->
      <main class="content">
        <router-view></router-view>
      </main>
    </div>

    <!-- 底部菜单栏 -->
    <footer class="bottom-menu">
      <span>用户信息</span>
      <span>时间：{{ new Date().toLocaleTimeString() }}</span>
      <span>网络状态：在线</span>
    </footer>
  </div>
</template>

<style scoped>
.app-container {
  display: flex;
  flex-direction: column;
  height: 100vh;
}

.top-menu {
  display: flex;
  justify-content: space-around;
  padding: 10px;
  background-color: #007bff;
  color: white;
}

.main-layout {
  display: flex;
  flex: 1;
}

.side-menu {
  width: 200px;
  background-color: #f0f0f0;
  padding: 10px;
}

.content {
  flex: 1;
  padding: 20px;
}

.bottom-menu {
  display: flex;
  justify-content: space-around;
  padding: 10px;
  background-color: #343a40;
  color: white;
}
</style>
