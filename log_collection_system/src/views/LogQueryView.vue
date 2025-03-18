<script setup>
import { ref } from 'vue';

const keyword = ref('');
const field = ref('all');
const startTime = ref('');
const endTime = ref('');
const logs = ref([
  { id: 1, level: 'INFO', message: '系统启动成功', timestamp: '2025-03-18 12:00:00', details: '详细日志内容...' },
  { id: 2, level: 'ERROR', message: '数据库连接失败', timestamp: '2025-03-18 12:10:00', details: '错误详情...' },
]);
const selectedLog = ref(null);

const searchLogs = () => {
  console.log('搜索日志', { keyword: keyword.value, field: field.value, startTime: startTime.value, endTime: endTime.value });
  // TODO: 调用 API 获取日志数据
};
</script>

<template>
  <div class="container">
    <h1 class="title">日志查询</h1>
    <div class="search-form">
      <input v-model="keyword" placeholder="关键字" class="input" />
      <select v-model="field" class="select">
        <option value="all">所有字段</option>
        <option value="level">日志级别</option>
        <option value="message">日志内容</option>
      </select>
      <input v-model="startTime" type="datetime-local" class="input" />
      <input v-model="endTime" type="datetime-local" class="input" />
      <button @click="searchLogs" class="button">搜索</button>
    </div>

    <table class="table">
      <thead>
        <tr>
          <th>ID</th>
          <th>级别</th>
          <th>内容</th>
          <th>时间</th>
          <th>操作</th>
        </tr>
      </thead>
      <tbody>
        <tr v-for="log in logs" :key="log.id">
          <td>{{ log.id }}</td>
          <td>{{ log.level }}</td>
          <td>{{ log.message }}</td>
          <td>{{ log.timestamp }}</td>
          <td>
            <button @click="selectedLog = log" class="detail-button">查看详情</button>
          </td>
        </tr>
      </tbody>
    </table>

    <!-- Log Details Dialog -->
    <div v-if="selectedLog" class="dialog">
      <div class="dialog-content">
        <h2>日志详情</h2>
        <p><strong>级别:</strong> {{ selectedLog.level }}</p>
        <p><strong>时间:</strong> {{ selectedLog.timestamp }}</p>
        <p><strong>内容:</strong> {{ selectedLog.message }}</p>
        <p><strong>详细信息:</strong> {{ selectedLog.details }}</p>
        <button @click="selectedLog = null" class="close-button">关闭</button>
      </div>
    </div>
  </div>
</template>

<style scoped>
.container {
  padding: 20px;
}

.title {
  font-size: 24px;
  font-weight: bold;
  margin-bottom: 20px;
}

.search-form {
  display: grid;
  grid-template-columns: repeat(4, 1fr);
  gap: 10px;
  margin-bottom: 20px;
}

.input,
.select {
  padding: 8px;
  border: 1px solid #ccc;
  border-radius: 4px;
}

.button {
  background-color: #4caf50;
  color: white;
  border: none;
  padding: 8px 16px;
  border-radius: 4px;
  cursor: pointer;
}

.button:hover {
  background-color: #45a049;
}

.table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 20px;
}

.table th,
.table td {
  border: 1px solid #ccc;
  padding: 8px;
  text-align: left;
}

.table th {
  background-color: #f4f4f4;
}

.detail-button {
  background-color: #2196f3;
  color: white;
  border: none;
  padding: 4px 8px;
  border-radius: 4px;
  cursor: pointer;
}

.detail-button:hover {
  background-color: #1976d2;
}

.dialog {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  justify-content: center;
  align-items: center;
}

.dialog-content {
  background-color: white;
  padding: 20px;
  border-radius: 8px;
  width: 300px;
  text-align: left;
}

.close-button {
  background-color: #f44336;
  color: white;
  border: none;
  padding: 6px 12px;
  border-radius: 4px;
  cursor: pointer;
  margin-top: 20px;
}

.close-button:hover {
  background-color: #e53935;
}
</style>
