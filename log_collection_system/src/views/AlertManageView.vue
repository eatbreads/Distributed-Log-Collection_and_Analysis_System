<template>
    <div>
      <h1>告警管理</h1>
      <p>这里是告警管理界面</p>
  
      <div class="filters">
        <label for="status">告警状态:</label>
        <select v-model="filter.status" id="status">
          <option value="all">全部</option>
          <option value="processed">已处理</option>
          <option value="unprocessed">待处理</option>
        </select>
  
        <label for="module">模块筛选:</label>
        <select v-model="filter.module" id="module">
          <option value="all">全部</option>
          <option value="database">数据库</option>
          <option value="system">系统</option>
          <option value="container">容器</option>
          <option value="network">网络</option>
          <option value="app">应用</option>
        </select>
  
        <label for="startDate">开始时间:</label>
        <input type="date" v-model="filter.startDate" id="startDate">
  
        <label for="endDate">结束时间:</label>
        <input type="date" v-model="filter.endDate" id="endDate">
  
        <button @click="applyFilters">应用筛选</button>
      </div>
  
      <div v-if="loading" class="loading">加载中...</div>
  
      <div v-else>
        <table>
          <thead>
            <tr>
              <th>告警ID</th>
              <th>告警类型</th>
              <th>模块</th>
              <th>时间</th>
              <th>状态</th>
              <th>处理人</th>
              <th>备注</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="alert in filteredAlerts" :key="alert.id">
              <td>{{ alert.id }}</td>
              <td>{{ alert.type }}</td>
              <td>{{ alert.module }}</td>
              <td>{{ alert.timestamp }}</td>
              <td>{{ alert.status }}</td>
              <td>{{ alert.handler }}</td>
              <td>{{ alert.remark }}</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>
  </template>
  
  <script setup>
  import { ref, computed, onMounted } from 'vue';
  // 从 .env 读取 API 地址
  const API_BASE_URL = import.meta.env.VITE_API_BASE_URL || "http://localhost:8089";
  // 假设这个是告警数据API的模拟函数
  const fetchAlerts = async (filters) => {
  try {
    const query = new URLSearchParams(filters).toString();
    const response = await fetch(`${API_BASE_URL}/alerts?${query}`, {
      method: 'GET',
      // headers: {
      //   'Content-Type': 'application/json',
      // },
    });
    
    if (!response.ok) {
      throw new Error(`HTTP error! Status: ${response.status}`);
    }
    
    return await response.json();
  } catch (error) {
    console.error('Error fetching alerts:', error);
    return [];
  }
};
  
  const filter = ref({
    status: 'all',
    module: 'all',
    startDate: '',
    endDate: ''
  });
  
  const alerts = ref([]);
  const loading = ref(true);
  
  const filteredAlerts = computed(() => {
    return alerts.value
      .filter(alert => {
        if (filter.value.status !== 'all' && alert.status !== filter.value.status) {
          return false;
        }
        if (filter.value.module !== 'all' && alert.module !== filter.value.module) {
          return false;
        }
        if (filter.value.startDate && alert.timestamp < filter.value.startDate) {
          return false;
        }
        if (filter.value.endDate && alert.timestamp > filter.value.endDate) {
          return false;
        }
        return true;
      })
      .sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp)); // 按时间降序排序
  });
  
  const applyFilters = async () => {
    loading.value = true;
    const data = await fetchAlerts(filter.value);
    alerts.value = data;
    loading.value = false;
  };
  
  onMounted(() => {
    applyFilters(); // 初始化时加载数据
  });
  </script>
  
  <style scoped>
  .filters {
    margin-bottom: 20px;
  }
  
  .filters label {
    margin-right: 10px;
  }
  
  .filters select,
  .filters input {
    margin-right: 20px;
  }
  
  table {
    width: 100%;
    border-collapse: collapse;
  }
  
  table th, table td {
    padding: 10px;
    border: 1px solid #ccc;
  }
  
  .loading {
    font-size: 16px;
    color: gray;
  }
  </style>
  